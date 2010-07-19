#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bCmdRenderPreview.h"
#include "bCmdRender.h"
#include "bReadShader.h"

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
using namespace yafaray;

std::map<string , yafaray::material_t*> renderPreview::materialMap;

MStatus renderPreview::doIt(const MArgList &args)
{
	MStatus stat;
	int size=400;
	
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
	previewShader.readShader(*yafPreview,materialMap);

	    yafPreview->paramsClearAll();
	    yafPreview->paramsSetString("type", "sphere");
		yafPreview->paramsSetPoint("center", 0, 0, 0);
		yafPreview->paramsSetFloat("radius", 2);
		yafPreview->paramsSetString("material", "yafGlass1");
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
		yafPreview->paramsSetInt("resx", size);
		yafPreview->paramsSetInt("resy", size);
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

		float* imageM = new float[size*size*4];
		yafaray::memoryIO_t memoryIO(size, size, imageM);

		yafPreview->paramsSetInt("width",size);
		yafPreview->paramsSetInt("height",size);
		yafPreview->paramsSetBool("z_channel",false);

		yafPreview->paramsSetString("background_name","world_background");

		yafPreview->render(memoryIO);
		yafPreview->clearAll();

		delete [] imageM;


	return stat;
}