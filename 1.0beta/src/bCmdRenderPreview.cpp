#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bCmdRenderPreview.h"
#include "bCmdRender.h"
#include "bReadShader.h"
#include "bReadObject.h"
#include "bReadCamera.h"

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MRenderView.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MObjectArray.h>
#include <maya/MIntArray.h>
#include <maya/MPlugArray.h>
#include <interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
using namespace yafaray;

std::map<string , yafaray::material_t*> renderPreview::materialMap;
std::map<string , yafaray::texture_t*> renderPreview::textureMap;

MStatus renderPreview::doIt(const MArgList &args)
{
	MStatus stat;

	int sizex=200;
	int sizey=200;
	
	yafrayInterface_t *yafPreview=renderScene::getyI();
	yafPreview->clearAll();
	yafPreview->startScene(1);

	//get gammainput
	MString listRnderSetting("ls -type yafRenderSetting");
	MStringArray listRenderResult;
	MGlobal::executeCommand(listRnderSetting,listRenderResult);

	MSelectionList list;
	MGlobal::getSelectionListByName(listRenderResult[0],list);
	float gammaInput;
	for(unsigned int index=0; index<list.length(); index++)
	{
		MObject renderSettingNode;
		list.getDependNode(index, renderSettingNode);
		MFnDependencyNode renderFn(renderSettingNode);
		renderFn.findPlug("GammaInput").getValue(gammaInput);
	}
	//done

	yafPreview->setInputGamma(gammaInput,true);

	getShader previewShader;
	previewShader.readShader(*yafPreview,materialMap,textureMap);

	//get the material name of the selected mesh
	MSelectionList selectObject;
	MStringArray names;
	MGlobal::getActiveSelectionList(selectObject);
	if (selectObject.length()==0)
	{
		MGlobal::displayError("(yafaray preview render) nothing selected, please select a mesh object!");
		return stat=MStatus::kFailure;
	}
	MItSelectionList selectMesh(selectObject,MFn::kMesh);
	for (; !selectMesh.isDone(); selectMesh.next())
	{

			MDagPath meshPath;
			selectMesh.getDagPath(meshPath);
			MFnMesh meshFn(meshPath);
			MObjectArray sgArray;
			MIntArray indexSG;
			meshFn.getConnectedShaders(0, sgArray, indexSG);
			MFnDependencyNode sgFn(sgArray[0]);
			MPlug surfacePlug=sgFn.findPlug("surfaceShader");
			MPlugArray sourceArray;
			if (surfacePlug.connectedTo(sourceArray, true, false))
			{
				MPlug sourcePlug=sourceArray[0];
				MObject sourceNode=sourcePlug.node();
				MFnDependencyNode sourceFn(sourceNode);
				names.append(sourceFn.name());
			}
	}
	//done
	if (names.length()==0)
	{
		MGlobal::displayError("(yafaray preview render) i think what you selected is not a mesh object!");
		return stat=MStatus::kFailure;
	}
	MString shaderName=names[0];
	//whether the material is yafaray's material
	//if not, no render....
	//MString doesn't have a function for judgement, use std
	MString nodeTypeCmd("nodeType "+shaderName);
	MString nodeTypeResult;
	MGlobal::executeCommand(nodeTypeCmd,nodeTypeResult);
	string shaderType(nodeTypeResult.asChar());
	std::string::size_type idx;
	idx=shaderType.find("yaf");	
	if (idx==std::string::npos)
	{
		MGlobal::displayError("(yafaray preview) please create a yafaray material first!");
		return MStatus::kFailure;
	}

	//if truely has yafaray material, open render veiw window, start render
	MGlobal::executeCommand("RenderViewWindow");

    yafPreview->paramsClearAll();
    yafPreview->paramsSetString("type", "sphere");
	yafPreview->paramsSetPoint("center", 0, 0, 0);
	yafPreview->paramsSetFloat("radius", 2);
	yafPreview->paramsSetString("material", shaderName.asChar());
	yafPreview->createObject("Sphere1");



	yafPreview->paramsClearAll();
	yafPreview->paramsSetColor("color", 1, 1, 1, 1);
	yafPreview->paramsSetPoint("from", 11, 3, 8);
	yafPreview->paramsSetFloat("power", 160);
	yafPreview->paramsSetString("type", "pointlight");
	yafPreview->createLight("LAMP1");

	yafPreview->paramsClearAll();
	yafPreview->paramsSetColor("color", 1, 1, 1, 1);
	yafPreview->paramsSetPoint("from", -2, -10, 2);
	yafPreview->paramsSetFloat("power", 18);
	yafPreview->paramsSetString("type", "pointlight");
	yafPreview->createLight("LAMP2");


	yafPreview->paramsClearAll();
	yafPreview->paramsSetString("type", "sunsky");
	yafPreview->paramsSetPoint("from", 1, 1, 1);
	yafPreview->paramsSetFloat("turbidity", 3);
	yafPreview->createBackground("world_background");


	yafPreview->paramsClearAll();
	yafPreview->paramsSetString("type", "perspective");
	yafPreview->paramsSetFloat("focal", 2.4);
	yafPreview->paramsSetPoint("from", 7, -7, 4.15);
	yafPreview->paramsSetPoint("up", 6.12392, -6.11394, 7.20305);
	yafPreview->paramsSetPoint("to", 4.89145, -4.88147, 2.90031);
	yafPreview->paramsSetInt("resx", sizex);
	yafPreview->paramsSetInt("resy", sizey);
	yafPreview->createCamera("cam");


	yafPreview->paramsClearAll();
	yafPreview->paramsSetString("type", "directlighting");
	yafPreview->createIntegrator("default");

	yafPreview->paramsClearAll();
	yafPreview->paramsSetString("type", "none");
	yafPreview->createIntegrator("volintegr");


	yafPreview->paramsClearAll();
	yafPreview->paramsSetString("camera_name", "cam");
	yafPreview->paramsSetString("integrator_name", "default");
	yafPreview->paramsSetString("volintegrator_name", "volintegr");

	yafPreview->paramsSetFloat("gamma", 1.8);
	yafPreview->paramsSetInt("AA_passes", 1);
	yafPreview->paramsSetInt("AA_minsamples", 1);
	yafPreview->paramsSetFloat("AA_pixelwidth", 1.5);
	yafPreview->paramsSetString("filter_type", "Mitchell");

	float* imageM = new float[sizex*sizey*4];
	yafaray::memoryIO_t memoryIO(sizex, sizey, imageM);

	yafPreview->paramsSetInt("width",sizex);
	yafPreview->paramsSetInt("height",sizey);
	yafPreview->paramsSetBool("z_channel",false);

	yafPreview->paramsSetString("background_name","world_background");

	yafPreview->render(memoryIO);
	toRenderView( sizex , sizey, imageM);
	yafPreview->clearAll();

	delete [] imageM;


	return stat;
}

MStatus renderPreview::toRenderView( const int sizex, const int sizey,  const float * imageM)
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
	for ( int i=0; i<sizey;i++)
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