#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bCmdRenderAnimation.h"
#include "bCmdRender.h"
#include"bReadShader.h"
#include"bReadObject.h"
#include"bReadLight.h"
#include"bReadCamera.h"
#include "bReadRender.h"
#include "bReadBackground.h"

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include "maya/MPoint.h"
#include <maya/MImage.h>
#include "maya/MIOStream.h"
#include <maya/MRenderView.h>
#include<interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include<map>
#include<string>
using namespace yafaray;
using namespace std;
std::map<string , yafaray::material_t*> renderAnimation::materialMap;
std::map<string, yafaray::texture_t*> renderAnimation::textureMap;

MStatus renderAnimation::doIt(const MArgList &args)
{
	MStatus stat;
	yafrayInterface_t * yafAnimation=renderScene::getyI();

	for (int i=1;i<200;i++)
	{
		MAnimControl::setCurrentTime(MTime(i,MTime::kFilm));

			yafAnimation->clearAll();
			yafAnimation->startScene();

			getShader shader;
			shader.readShader(*yafAnimation,materialMap,textureMap);
			cout<<"shader ok"<<endl;

			getObject object;
			object.readObject(*yafAnimation,materialMap);
			cout<<"obj ok"<<endl;

			getLight light;
			light.readLight(*yafAnimation);
			cout<<"light ok"<<endl;

			getCamera camera;
			camera.readCamera(*yafAnimation);
			cout<<"camera ok"<<endl;

			//world_background and volintegr
			getWorld world;
			world.readWorld(*yafAnimation);
			cout<<"world ok"<<endl;

			getRender renderSetting;
			renderSetting.createRender(*yafAnimation);
			cout<<"renderSetting ok"<<endl;

			int sizex, sizey;
			renderSetting.getImageWidth(sizex);
			renderSetting.getImageHeight(sizey);

			beginRender(*yafAnimation,sizex, sizey,i);
			yafAnimation->clearAll();

	}
	MGlobal::displayInfo("(yafaray animation render) yafaray render out");

	return stat;
}
MStatus renderAnimation::beginRender(yafrayInterface_t &yI,const int sizex, const int sizey,const int currentTime)
{
	MStatus stat=MStatus::kSuccess;

	float* imageM = new float[sizex*sizey*4];
	yafaray::memoryIO_t memoryIO(sizex,sizey,imageM);
	yI.render(memoryIO);
	cout<<"=================================="<<endl;
	cout<<"render test:beginRender"<<endl;
	renderToImage(sizex, sizey, imageM,currentTime);
	delete [] imageM;

	return stat;
}
MStatus renderAnimation::renderToImage(const int sizex, const int sizey, const float *imageM,const int currentTime)
{
	MStatus stat;

	RV_PIXEL *resultPixels=new RV_PIXEL[sizex*sizey];
	for (int index=0 ; index<(sizex*sizey) ; index++)
	{
		resultPixels[index].r=imageM[index*4+0]*255;
		resultPixels[index].g=imageM[index*4+1]*255;
		resultPixels[index].b=imageM[index*4+2]*255;
		resultPixels[index].a=imageM[index*4+3];
	}
	RV_PIXEL *resultPixelReverse= new RV_PIXEL[sizex*sizey];
	for (int i=0; i<sizey;i++)
	{
		for (int j=0; j<sizex; j++)
		{
			resultPixelReverse[i*sizex+j]=resultPixels[(sizey-1-i)*sizex+j];
		}
	}

	MImage resultImage;
	resultImage.create(sizex,sizey,4,MImage::kByte);
	int imageSize=sizex*sizey;

	unsigned char* Pixels=resultImage.pixels();

	for (int i=0; i<imageSize; i++)
	{
		*Pixels=std::max(0,std::min(255,(int)resultPixelReverse[i].r));
		Pixels++;
		*Pixels=std::max(0,std::min(255,(int)resultPixelReverse[i].g));
		Pixels++;
		*Pixels=std::max(0,std::min(255,(int)resultPixelReverse[i].b));
		Pixels++;
		*Pixels=std::max(0,std::min(255,(int)resultPixelReverse[i].a));
		Pixels++;		
	}

	delete [] resultPixels;
	delete [] resultPixelReverse;

	char c[20];
	sprintf(c, "%d",currentTime);
	std::string numString=c;
    const char * nString=numString.c_str();

	MString filePath("c:/test");
	MString fileFrame(nString);
	MString fileName(filePath+fileFrame+".tga");

	stat=resultImage.writeToFile(fileName,"tga");
	if (stat=MStatus::kFailure)
	{
		cout<<"can't write to the file"<<endl;
	}
	else
	{
		cout<<"write to file succeeded"<<endl;
	}

	resultImage.release();
	delete [] Pixels;
	MGlobal::displayInfo(fileFrame+"has been renderer out");


	return stat;
}