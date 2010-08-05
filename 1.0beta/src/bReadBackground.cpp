#define NOMINMAX
#define _USE_MATH_DEFINES 1
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
		MObject gradientColors;

		worldFn.findPlug("HorizonColor").getValue(gradientColors);
		MFnNumericData horizonData(gradientColors);
		float horizonR,horizonG,horizonB;
		horizonData.getData(horizonR,horizonG,horizonB);
		yI.paramsSetColor("horizon_color",horizonR,horizonG,horizonB);

		//MObject zenithColor;
		worldFn.findPlug("ZenithColor").getValue(gradientColors);
		MFnNumericData zenithData(gradientColors);
		float zenithR,zenithG,zenithB;
		zenithData.getData(zenithR,zenithG,zenithB);
		yI.paramsSetColor("zenith_color",zenithR,zenithG,zenithB);

		//MObject horGround;
		worldFn.findPlug("HorGroundColor").getValue(gradientColors);
		MFnNumericData hGData(gradientColors);
		float hgR,hgG,hgB;
		hGData.getData(hgR,hgG,hgB);
		yI.paramsSetColor("horizon_ground_color",hgR,hgG,hgB);

		//MObject zenGround;
		worldFn.findPlug("ZenGroundColor").getValue(gradientColors);
		MFnNumericData zGData(gradientColors);
		float zgR,zgG,zgB;
		zGData.getData(zgR,zgG,zgB);
		yI.paramsSetColor("zenith_ground_color",zgR,zgG,zgB);

		float power;
		worldFn.findPlug("BackgroundPower").getValue(power);
		yI.paramsSetFloat("power",power);

		bool ibl;
		worldFn.findPlug("UseIBL").getValue(ibl);
		yI.paramsSetBool("ibl",ibl);

		int iblSamples;
		worldFn.findPlug("IBLSamples").getValue(iblSamples);
		yI.paramsSetInt("ibl_samples",iblSamples);

		yI.paramsSetString("type","gradientback");
	}
	//texture
	else if (backgroundType==2)
	{
	}
	//sunsky
	else if (backgroundType==3)
	{
		double x,y,z;
		worldFn.findPlug("xDirection").getValue(x);
		worldFn.findPlug("yDirection").getValue(y);
		worldFn.findPlug("zDirection").getValue(z);
		yI.paramsSetPoint("from",x,y,z);

		float turbidity;
		worldFn.findPlug("Turbidity").getValue(turbidity);
		yI.paramsSetFloat("turbidity",turbidity);

		float abcde;
		worldFn.findPlug("AHorBrght").getValue(abcde);
		yI.paramsSetFloat("a_var",abcde);

		//float b;
		worldFn.findPlug("BHorSprd").getValue(abcde);
		yI.paramsSetFloat("b_var",abcde);

		//float c;
		worldFn.findPlug("CSunBrght").getValue(abcde);
		yI.paramsSetFloat("c_var",abcde);

		//float d;
		worldFn.findPlug("DSunsize").getValue(abcde);
		yI.paramsSetFloat("d_var",abcde);

		//float e;
		worldFn.findPlug("EBacklight").getValue(abcde);
		yI.paramsSetFloat("e_var",abcde);

		bool addSun;
		worldFn.findPlug("AddRealSun").getValue(addSun);
		yI.paramsSetBool("add_sun",addSun);

		float sunPower;
		worldFn.findPlug("SunPower").getValue(sunPower);
		yI.paramsSetFloat("sun_power",sunPower);

		bool backgourndLight;
		worldFn.findPlug("Skylight").getValue(backgourndLight);
		yI.paramsSetBool("background_light",backgourndLight);

		int lightSamples;
		worldFn.findPlug("SkySamples").getValue(lightSamples);
		yI.paramsSetInt("light_samples",lightSamples);

		float power;
		worldFn.findPlug("BackgroundPower").getValue(power);
		yI.paramsSetFloat("power",power);

		yI.paramsSetString("type","sunsky");
	}
	//darky's sunsky
	else if (backgroundType==4)
	{
		double x,y,z;
		worldFn.findPlug("xDirection").getValue(x);
		worldFn.findPlug("yDirection").getValue(y);
		worldFn.findPlug("zDirection").getValue(z);
		yI.paramsSetPoint("from",x,y,z);

		float dsFloat;
		int dsInt;
		bool dsBool;

		worldFn.findPlug("DarkTideTurbidity").getValue(dsFloat);
		yI.paramsSetFloat("turbidity",dsFloat);

		worldFn.findPlug("Altitude").getValue(dsFloat);
		yI.paramsSetFloat("altitude",dsFloat);

		worldFn.findPlug("BrightnessOfHorizonGradient").getValue(dsFloat);
		yI.paramsSetFloat("a_var",dsFloat);

		worldFn.findPlug("LuminanceOfHorizon").getValue(dsFloat);
		yI.paramsSetFloat("b_var",dsFloat);

		worldFn.findPlug("SolarRegionIntensity").getValue(dsFloat);
		yI.paramsSetFloat("c_var",dsFloat);

		worldFn.findPlug("WidthOfCircumsolarRegion").getValue(dsFloat);
		yI.paramsSetFloat("d_var",dsFloat);

		worldFn.findPlug("BackgroundLight").getValue(dsFloat);
		yI.paramsSetFloat("e_var",dsFloat);

		//clampRGB......
		//why read this from render node, i'd like to create another in the world setting node....
		worldFn.findPlug("ClampRGB").getValue(dsBool);
		yI.paramsSetBool("clamp_rgb",dsBool);

		worldFn.findPlug("AddDSRealSun").getValue(dsBool);
		yI.paramsSetBool("add_sun",dsBool);

		worldFn.findPlug("DSSunPower").getValue(dsFloat);
		yI.paramsSetFloat("sun_power",dsFloat);

		worldFn.findPlug("AddDSSkyLight").getValue(dsBool);
		yI.paramsSetBool("background_light",dsBool);

		worldFn.findPlug("DSSkyPower").getValue(dsFloat);
		yI.paramsSetFloat("power",dsFloat);

		worldFn.findPlug("CausticPhotons").getValue(dsBool);
		yI.paramsSetBool("with_caustic",dsBool);

		worldFn.findPlug("DiffusePhotons").getValue(dsBool);
		yI.paramsSetBool("with_diffuse",dsBool);

		worldFn.findPlug("DSSkySamples").getValue(dsInt);
		yI.paramsSetInt("light_samples",dsInt);

		worldFn.findPlug("Brightness").getValue(dsFloat);
		yI.paramsSetFloat("bright",dsFloat);

		worldFn.findPlug("Night").getValue(dsBool);
		yI.paramsSetBool("night",dsBool);

		worldFn.findPlug("Exposure").getValue(dsFloat);
		yI.paramsSetFloat("exposure",dsFloat);

		worldFn.findPlug("GammaEncoding").getValue(dsBool);
		yI.paramsSetBool("gamma_enc",dsBool);

		short colorSpace;
		worldFn.findPlug("DarkTideSunskyColorSpaces").getValue(colorSpace);
		switch(colorSpace)
		{
		    case 0:
				yI.paramsSetString("color_space","CIE (E)");
				break;
			case 1:
				yI.paramsSetString("color_space","CIE (D50)");
				break;
			case 2:
				yI.paramsSetString("color_space","sRBG (D65)");
				break;
			case 3:
				yI.paramsSetString("color_space","sRGB (D50)");
				break;
			default:
				break;
		}

		yI.paramsSetString("type","darksky");
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