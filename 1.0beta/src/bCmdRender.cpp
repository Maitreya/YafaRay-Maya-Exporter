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
MStatus renderScene::doIt(const MArgList &args)
{
	MStatus stat=MStatus::kSuccess;
	int sizex=640;
	int sizey=480;

	//get gammainput
	//MString listRnderSetting("ls -type yafRenderSetting");
	//MStringArray listRenderResult;
	//MGlobal::executeCommand(listRnderSetting,listRenderResult);

	//MSelectionList list;
	//MGlobal::getSelectionListByName(listRenderResult[0],list);
	//float gammaInput;
	//for(unsigned int index=0; index<list.length(); index++)
	//{
	//	MObject renderSettingNode;
	//	list.getDependNode(index, renderSettingNode);
	//	MFnDependencyNode renderFn(renderSettingNode);
	//	renderFn.findPlug("GammaInput").getValue(gammaInput);
	//}
	//done

	yI.clearAll();
	yI.startScene();

	//yI.setInputGamma(gammaInput,true);

	getShader shader;
	shader.readShader(yI,materialMap);
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

	//test
	//yI.paramsClearAll();
	//yI.paramsSetString("type", "sunsky");
	//yI.paramsSetPoint("from", 1, 1, 1);
	//yI.paramsSetFloat("turbidity", 3);
	//yI.createBackground("world_background");

	//yI.paramsClearAll();
	//yI.paramsSetString("type", "none");
	//yI.createIntegrator("volintegr");

	getRender renderSetting;
	renderSetting.readRender(yI);
	cout<<"renderSetting ok"<<endl;

	//test
	//yI.paramsClearAll();
	//yI.paramsSetString("type", "directlighting");
	//yI.createIntegrator("default");

	yI.paramsClearAll();
	yI.paramsSetString("camera_name", "cam");
	yI.paramsSetString("integrator_name", "default");
	yI.paramsSetString("volintegrator_name", "volintegr");
	yI.paramsSetString("background_name","world_background");

	yI.paramsSetFloat("gamma", 1.8);
	yI.paramsSetInt("AA_passes", 1);
	yI.paramsSetInt("AA_minsamples", 1);
	yI.paramsSetFloat("AA_pixelwidth", 1.5);
	yI.paramsSetString("filter_type", "Mitchell");

	yI.paramsSetInt("width",sizex);
	yI.paramsSetInt("height",sizey);
	yI.paramsSetBool("z_channel",false);

	beginRender(sizex, sizey);
	yI.clearAll();
	MGlobal::displayInfo("yafaray render out");

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
	for (unsigned int index=0 ; index<(sizex*sizey) ; index++)
	{
		resultPixels[index].r=imageM[index*4+0]*255;
		resultPixels[index].g=imageM[index*4+1]*255;
		resultPixels[index].b=imageM[index*4+2]*255;
		resultPixels[index].a=imageM[index*4+3];
	}
	RV_PIXEL *resultPixelReverse= new RV_PIXEL[sizex*sizey];
	for (unsigned int i=0; i<sizey;i++)
	{
		for (unsigned int j=0; j<sizex; j++)
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