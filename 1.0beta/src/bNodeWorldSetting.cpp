#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeWorldSetting.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

const MTypeId worldSettingNode::id(0x75301);

	  MObject worldSettingNode::backgroundTypes;
	  MObject worldSettingNode::volumeInitTypes;
	  MObject worldSettingNode::DSSkyColorSpaces;

	//single color attribute
	  MObject worldSettingNode::backgroundColor;

	//gradient color attribute
	  MObject worldSettingNode::horizonColor;
	  MObject worldSettingNode::zenithColor;
	  MObject worldSettingNode::horGroundColor;
	  MObject worldSettingNode::zenGroundColor;

	//texture attribute
	  MObject worldSettingNode::texRotation;

	//sunsky attribute
	  MObject worldSettingNode::turbidity;
	  MObject worldSettingNode::AHorBrght;
	  MObject worldSettingNode::BHorSprd;
	  MObject worldSettingNode::CSunBrght;
	  MObject worldSettingNode::DSunSize;
	  MObject worldSettingNode::EBacklight;
	  MObject worldSettingNode::xDirection;
	  MObject worldSettingNode::yDirection;
	  MObject worldSettingNode::zDirection;
	  MObject worldSettingNode::realSun;
	  MObject worldSettingNode::sunPower;
	  MObject worldSettingNode::skyLight;
	  MObject worldSettingNode::skySamples;

	//DarkTide's sunsky attribute
	  MObject worldSettingNode::DSClampRGB;

	  MObject worldSettingNode::DSTurbidity;
	  MObject worldSettingNode::renderDSA;
	  MObject worldSettingNode::renderDSB;
	  MObject worldSettingNode::renderDSC;
	  MObject worldSettingNode::renderDSD;
	  MObject worldSettingNode::renderDSE;
	//sun direction shared with sunsky
	  MObject worldSettingNode::DSAltitude;
	  MObject worldSettingNode::DSNight;
	  MObject worldSettingNode::DSRealSun;
	  MObject worldSettingNode::DSSunPower;
	  MObject worldSettingNode::DSSkyLight;
	  MObject worldSettingNode::DSSkyPower;
	  MObject worldSettingNode::DSSkySamples;
	  MObject worldSettingNode::DSSkyBright;
	  MObject worldSettingNode::DSExposure;
	  MObject worldSettingNode::DSGammaEncoding;

	//these  appear in each type of background
	  MObject worldSettingNode::useIBL;
	  MObject worldSettingNode::IBLSamples;
	  MObject worldSettingNode::backgroundDiffuse;
	  MObject worldSettingNode::backgoundCaustic;
	  MObject worldSettingNode::backgoundPower;

	//volume settings
	MObject worldSettingNode::volumeStepSize;
	MObject worldSettingNode::volumeAdaptive;
	MObject worldSettingNode::volumeOptimize;
	MObject worldSettingNode::volumeAttMapScale;
    MObject worldSettingNode::volumeSkyST;
	MObject worldSettingNode::volumeAlpha;

	//the node needs an output, even we dont need it...
	  MObject worldSettingNode::worldOutput;

MStatus worldSettingNode::compute(const MPlug &plug, MDataBlock &data)
{
		  return MStatus::kSuccess;
}

void *worldSettingNode::creator()
{
	return new worldSettingNode;
}

MStatus worldSettingNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	backgroundTypes=enumAttr.create("BackgroundTypes","wbaty",0);
	enumAttr.addField("Single Color",0);
	enumAttr.addField("Gradient",1);
	enumAttr.addField("Texture",2);
	enumAttr.addField("Sunsky",3);
	enumAttr.addField("DarkTide's Sunsky",4);
	//MCHECKERROR(stat, "create background types");
	enumAttr.setKeyable(true);
	enumAttr.setStorable(true);
	enumAttr.setHidden(true);

	volumeInitTypes=enumAttr.create("VolumeTypes","wvoty",0);
	enumAttr.addField("None",0);
	enumAttr.addField("Single Scatter",1);
    enumAttr.addField("Sky",2);
	//MCHECKERROR(stat, "create volume types");
	enumAttr.setKeyable(true);
	enumAttr.setStorable(true);

	DSSkyColorSpaces=enumAttr.create("DarkTideSunskyColorSpaces","wdasu",0);
	enumAttr.addField("CIEE",0);
	enumAttr.addField("CIED50",1);
	enumAttr.addField("sRBGD65",2);
	enumAttr.addField("sRGBD50",3);
	//MCHECKERROR(stat, "create DarkTide's sunsky color spaces");
	enumAttr.setKeyable(true);
	enumAttr.setStorable(true);

	backgroundColor=numAttr.createColor("BackgroundColor","wbaco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.0,0.0,0.0);

	//gradient attribute
	horizonColor=numAttr.createColor("HorizonColor","whoco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.1,0.8,0.5);

	zenithColor=numAttr.createColor("ZenithColor","wzeco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.4,0.5,0.1);

	horGroundColor=numAttr.createColor("HorGroundColor","whgco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.4,0.5,0.6);

	zenGroundColor=numAttr.createColor("ZenGroundColor","wzgco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.9,0.5,0.2);

	//texture attribute
	texRotation=numAttr.create("TextureRotation","wtero",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(360.0f);

	//sunsky attribute
	turbidity=numAttr.create("Turbidity","wtu",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1.0f);
	numAttr.setMax(20.0f);

	AHorBrght=numAttr.create("AHorBrght","wahb",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10.0f);

	BHorSprd=numAttr.create("BHorSprd","wbhs",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10.0f);

	CSunBrght=numAttr.create("CSunBrght","wcsb",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10.0f);

	DSunSize=numAttr.create("DSunsize","wdss",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10.0f);

	EBacklight=numAttr.create("EBacklight","webl",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10.0f);

	xDirection=numAttr.create("xDirection","wxd",MFnNumericData::kDouble,0.000);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-1.000);
	numAttr.setMax(1.000);


	yDirection=numAttr.create("yDirection","wyd",MFnNumericData::kDouble,0.000);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-1.000);
	numAttr.setMax(1.000);

	zDirection=numAttr.create("zDirection","wzd",MFnNumericData::kDouble,0.000);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-1.000);
	numAttr.setMax(1.000);

	realSun=numAttr.create("AddRealSun","wads",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	sunPower=numAttr.create("SunPower","wsupo",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10.0);

	skyLight=numAttr.create("Skylight","wsk",MFnNumericData::kBoolean,1);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	skySamples=numAttr.create("SkySamples","wsksa",MFnNumericData::kInt,1);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1);
	numAttr.setMax(128);

	//DarkTide's sunsky attribute
	DSClampRGB=numAttr.create("ClampRGB","wcrgb",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	DSTurbidity=numAttr.create("DarkTideTurbidity","wdatu",MFnNumericData::kFloat,2.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(2.0);
	numAttr.setMax(12.0);

	renderDSA=numAttr.create("BrightnessOfHorizonGradient","wbhg",MFnNumericData::kFloat,-10.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	renderDSB=numAttr.create("LuminanceOfHorizon","wloh",MFnNumericData::kFloat,-10.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	renderDSC=numAttr.create("SolarRegionIntensity","wsri",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(50.0);

	renderDSD=numAttr.create("WidthOfCircumsolarRegion","wwocr",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(50.0);

	renderDSE=numAttr.create("BackgroundLight","wbl",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-30.0);
	numAttr.setMax(30.0);

	//sun direction shared with sunsky
	 DSAltitude=numAttr.create("Altitude","wal",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-1.0);
	numAttr.setMax(2.0);

	DSNight=numAttr.create("Night","wni",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	 DSRealSun=numAttr.create("AddDSRealSun","wadrs",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 DSSunPower=numAttr.create("DSSunPower","wdsp",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(10.0);

	 DSSkyLight=numAttr.create("AddDSSkyLight","wadsl",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 DSSkyPower=numAttr.create("DSSkyPower","wdssp",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(10000.0);

	 DSSkySamples=numAttr.create("DSSkySamples","wdsss",MFnNumericData::kInt,1);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(256);

	 DSSkyBright=numAttr.create("Brightness","wbr",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(10.0);

	 DSExposure=numAttr.create("Exposure","wex",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(10.0);

	 DSGammaEncoding=numAttr.create("GammaEncoding","wgaen",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	//these  appear in each type of background
	 useIBL=numAttr.create("UseIBL","wibl",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 IBLSamples=numAttr.create("IBLSamples","wibls",MFnNumericData::kInt,1);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(512);

	 backgroundDiffuse=numAttr.create("DiffusePhotons","wdiph",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 backgoundCaustic=numAttr.create("CausticPhotons","wcaph",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 backgoundPower=numAttr.create("BackgroundPower","wbapo",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(10000.0);

	 volumeStepSize=numAttr.create("StepSize","wvstsi",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(100.0);

	volumeAdaptive=numAttr.create("Adaptive","wvad",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	volumeOptimize=numAttr.create("Optimize","wvop",MFnNumericData::kBoolean,0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	volumeAttMapScale=numAttr.create("AttGridResolution","wvagr",MFnNumericData::kInt,1);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(50);

	 volumeSkyST=numAttr.create("Scale","wvsc",MFnNumericData::kDouble,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0001);
	 numAttr.setMax(10.000);

	volumeAlpha=numAttr.create("Alpha","wval",MFnNumericData::kDouble,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0001);
	 numAttr.setMax(10.000);

	//the node needs an output, even we dont need it...
	worldOutput=numAttr.create("WorldSettingOutput","wwso",MFnNumericData::kBoolean);
	numAttr.setDefault(true);
	numAttr.setHidden(true);

	setAttribute();


	return stat;
}

void worldSettingNode::setAttribute()
{
	addAttribute(backgroundTypes);
	addAttribute(volumeInitTypes);
	addAttribute(DSSkyColorSpaces);

	addAttribute(backgroundColor);

	addAttribute(horizonColor);
	addAttribute(zenithColor);
	addAttribute(horGroundColor);
	addAttribute(zenGroundColor);

	addAttribute(texRotation);

	addAttribute(turbidity);
	addAttribute(AHorBrght);
	addAttribute(BHorSprd);
	addAttribute(CSunBrght);
	addAttribute(DSunSize);
	addAttribute(EBacklight);

	addAttribute(xDirection);
	addAttribute(yDirection);
	addAttribute(zDirection);
	addAttribute(realSun);
	addAttribute(sunPower);
	addAttribute(skyLight);
	addAttribute(skySamples);

	//DarkTide's sunsky attribute
	addAttribute(DSClampRGB);
	addAttribute(DSTurbidity);
	addAttribute(renderDSA);
	addAttribute(renderDSB);
	addAttribute(renderDSC);
	addAttribute(renderDSD);
	addAttribute(renderDSE);
	//sun direction shared with sunsky
	addAttribute(DSAltitude);
	addAttribute(DSNight);
	addAttribute(DSRealSun);
	addAttribute(DSSunPower);
	addAttribute(DSSkyLight);
	addAttribute(DSSkyPower);
	addAttribute(DSSkySamples);
	addAttribute(DSSkyBright);
	addAttribute(DSExposure);
	addAttribute(DSGammaEncoding);

	//these  appear in each type of background
	addAttribute(useIBL);
	addAttribute(IBLSamples);
	addAttribute(backgroundDiffuse);
	addAttribute(backgoundCaustic);
	addAttribute(backgoundPower);

	addAttribute(volumeStepSize);
	addAttribute(volumeAdaptive);
	addAttribute(volumeOptimize);
	addAttribute(volumeAttMapScale);
    addAttribute(volumeSkyST);
	addAttribute(volumeAlpha);

	//the node needs an output, even we dont need it...
	addAttribute(worldOutput);

	attributeAffects(backgroundTypes,worldOutput);
	attributeAffects(volumeInitTypes,worldOutput);
	attributeAffects(DSSkyColorSpaces,worldOutput);
	attributeAffects(backgroundColor,worldOutput);
	attributeAffects(horizonColor,worldOutput);
	attributeAffects(zenithColor,worldOutput);
	attributeAffects(horGroundColor,worldOutput);
	attributeAffects(zenGroundColor,worldOutput);
	attributeAffects(texRotation,worldOutput);
	attributeAffects(turbidity,worldOutput);
	attributeAffects(AHorBrght,worldOutput);
	attributeAffects(BHorSprd,worldOutput);
	attributeAffects(CSunBrght,worldOutput);
	attributeAffects(DSunSize,worldOutput);
	attributeAffects(EBacklight,worldOutput);
	attributeAffects(xDirection,worldOutput);
	attributeAffects(yDirection,worldOutput);
	attributeAffects(zDirection,worldOutput);
	attributeAffects(realSun,worldOutput);
	attributeAffects(sunPower,worldOutput);
	attributeAffects(skyLight,worldOutput);
	attributeAffects(skySamples,worldOutput);


	attributeAffects(DSClampRGB,worldOutput);
	attributeAffects(DSTurbidity,worldOutput);
	attributeAffects(renderDSA,worldOutput);
	attributeAffects(renderDSB,worldOutput);
	attributeAffects(renderDSC,worldOutput);
	attributeAffects(renderDSD,worldOutput);
	attributeAffects(renderDSE,worldOutput);
	//sun direction shared with sunsky
	attributeAffects(DSAltitude,worldOutput);
	attributeAffects(DSNight,worldOutput);
	attributeAffects(DSRealSun,worldOutput);
	attributeAffects(DSSunPower,worldOutput);
	attributeAffects(DSSkyLight,worldOutput);
	attributeAffects(DSSkyPower,worldOutput);
	attributeAffects(DSSkySamples,worldOutput);
	attributeAffects(DSSkyBright,worldOutput);
	attributeAffects(DSExposure,worldOutput);
	attributeAffects(DSGammaEncoding,worldOutput);

	//these  appear in each type of background
	attributeAffects(useIBL,worldOutput);
	attributeAffects(IBLSamples,worldOutput);
	attributeAffects(backgroundDiffuse,worldOutput);
	attributeAffects(backgoundCaustic,worldOutput);
	attributeAffects(backgoundPower,worldOutput);

	attributeAffects(volumeStepSize,worldOutput);
	attributeAffects(volumeAdaptive,worldOutput);
	attributeAffects(volumeOptimize,worldOutput);
	attributeAffects(volumeAttMapScale,worldOutput);
    attributeAffects(volumeSkyST,worldOutput);
	attributeAffects(volumeAlpha,worldOutput);
}

