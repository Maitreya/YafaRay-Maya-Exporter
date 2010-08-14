#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bCmdRender.h"
#include"bReadShader.h"
#include"bReadObject.h"
#include"bReadLight.h"
#include"bReadCamera.h"
#include "bReadRender.h"
#include "bReadBackground.h"

#include <maya/MGlobal.h>
#include <maya/MRenderView.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include<interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
#include<map>
#include<string>
using namespace yafaray;
//using namespace std;
yafrayInterface_t renderScene::yI;
std::map<string , yafaray::material_t*> renderScene::materialMap;
std::map<string , yafaray::texture_t*> renderScene::textureMap;
MStatus renderScene::doIt(const MArgList &args)
{
	MStatus stat=MStatus::kSuccess;
	MGlobal::executeCommand("RenderViewWindow");

	yI.clearAll();
	yI.startScene();

	//yI.setInputGamma(gammaInput,true);

	getShader shader;
	shader.readTexture(yI,textureMap);
	shader.readShader(yI,materialMap,textureMap);
	cout<<"shader ok"<<endl;

	getObject object;
	object.readObject(yI,materialMap);
	cout<<"obj ok"<<endl;

	getLight light;
	light.readLight(yI);
	cout<<"light ok"<<endl;

	getCamera camera;
	camera.readCamera(yI);
	cout<<"camera ok"<<endl;

	//world_background and volintegr
	getWorld world;
	world.readWorld(yI);
	cout<<"world ok"<<endl;

	getRender renderSetting;
	renderSetting.createRender(yI);
	cout<<"renderSetting ok"<<endl;

	getRender renderData;
	int sizex, sizey;
	renderData.getImageWidth(sizex);
	renderData.getImageHeight(sizey);

	beginRender(sizex, sizey);
	yI.clearAll();
	MGlobal::displayInfo("(yafaray render) yafaray render out");

	return stat;
}
MStatus renderScene::beginRender(const int sizex, const int sizey)
{
	MStatus stat=MStatus::kSuccess;

    float* imageM = new float[sizex*sizey*4];
	yafaray::memoryIO_t memoryIO(sizex,sizey,imageM);
	yI.render(memoryIO);
	renderToView(sizex, sizey, imageM);
	delete [] imageM;

	return stat;
}
MStatus renderScene::renderToView(const int sizex, const int sizey, const float *imageM)
{
	MStatus stat;

	if (!MRenderView::doesRenderEditorExist())
	{
		MGlobal::displayError("can't push pixels to render view, cos' maya is running on batch mode");
		return stat=MStatus::kFailure;
	}
	if (MRenderView::startRender(sizex, sizey , false, true)!= MStatus::kSuccess)
	{
		MGlobal::displayError("errors occurred when rendering start!");
		return stat=MStatus::kFailure;
	}



	//get pixels from yafaray image memory
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

	if (MRenderView::updatePixels(0 , sizex-1, 0 , sizey-1, resultPixelReverse)!=MStatus::kSuccess)
	{
		MGlobal::displayError("errors occurred when updating pixels!");
		return stat=MStatus::kFailure;
	}
	delete [] resultPixels;
	delete [] resultPixelReverse;
	if (MRenderView::refresh(0 , sizex-1, 0 , sizey-1)!=MStatus::kSuccess)
	{
		MGlobal::displayError("errors occurred when refresh!");
		return stat=MStatus::kFailure;
	}

	if (MRenderView::endRender()!=MStatus::kSuccess)
	{
		MGlobal::displayError("errors occurred when ending render!");
		return stat=MStatus::kFailure;
	}

	return stat;
}
yafrayInterface_t* renderScene::getyI()
{
	return &yI;
}