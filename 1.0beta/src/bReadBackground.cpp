#include "bReadBackground.h"

#include <maya/MString.h>
#include<maya/MStringArray.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericData.h>
#include <maya/MPlug.h>
#include <interface/yafrayinterface.h>
using namespace yafaray;

MStatus getWorld::readWorld(yafaray::yafrayInterface_t &yI)
{
	MStatus stat;
	MString listWorld("ls -type yafWorldSetting");
	MStringArray listWorldResult;
	MGlobal::executeCommand(listWorld,listWorldResult);

	MSelectionList list;
	MGlobal::getSelectionListByName(listWorldResult[0],list);
	for (unsigned int index=0; index<list.length(); index++)
	{
		MObject worldSettingNode;
		list.getDependNode(index,worldSettingNode);
		MFnDependencyNode worldFn(worldSettingNode);

		readBackground(yI, worldFn);
		readVolumeIntegrator(yI, worldFn);
	}

	return stat;
}
MStatus getWorld::readBackground(yafaray::yafrayInterface_t &yI, MFnDependencyNode &worldFn)
{
	MStatus stat;

	yI.paramsClearAll();

	short backgroundType;
	worldFn.findPlug("BackgroundTypes").getValue(backgroundType);
	//background type setting
	//single color
	if (backgroundType==0)
	{
		MObject bgColor;
		worldFn.findPlug("BackgroundColor").getValue(bgColor);
		MFnNumericData bgcData(bgColor);
		float bgR,bgG,bgB;
		bgcData.getData(bgR,bgG,bgB);
		yI.paramsSetColor("color",bgR,bgG,bgB);

		bool ibl;
		worldFn.findPlug("UseIBL").getValue(ibl);
		yI.paramsSetBool("ibl",ibl);

		int iblSamples;
		worldFn.findPlug("IBLSamples").getValue(iblSamples);
		yI.paramsSetInt("ibl_samples",iblSamples);

		float power;
		worldFn.findPlug("BackgroundPower").getValue(power);
		yI.paramsSetFloat("power",power);

		yI.paramsSetString("type","constant");

	}
	//gradient
	else if (backgroundType==1)
	{
	}
	//texture
	else if (backgroundType==2)
	{
	}
	//sunsky
	else if (backgroundType==3)
	{
	}
	//darky's sunsky
	else if (backgroundType==4)
	{
	}

	yI.createBackground("world_background");

	return stat;
}
MStatus getWorld::readVolumeIntegrator(yafaray::yafrayInterface_t &yI, MFnDependencyNode &worldFn)
{
	MStatus stat;

	yI.paramsClearAll();

	short volumeIntegratorType;
	worldFn.findPlug("VolumeTypes").getValue(volumeIntegratorType);
	//none
	if (volumeIntegratorType==0)
	{
		yI.paramsSetString("type","none");
	}
	//single scatter
	else if (volumeIntegratorType==1)
	{
		yI.paramsSetString("type","SingleScatterIntegrator");
		//strange....step size is in the "none" frame in gui
		float stepSize;
		worldFn.findPlug("StepSize").getValue(stepSize);
		yI.paramsSetFloat("stepSize",stepSize);

		bool adaptive;
		worldFn.findPlug("Adaptive").getValue(adaptive);
		yI.paramsSetBool("adaptive",adaptive);

		bool optimize;
		worldFn.findPlug("Optimize").getValue(optimize);
		yI.paramsSetBool("optimize",optimize);

	}
	//sky
	else if (volumeIntegratorType==2)
	{
		yI.paramsSetString("type","SkyIntegrator");
		//parameters a great shit here.....
	}

	yI.createIntegrator("volintegr");

	return stat;
}