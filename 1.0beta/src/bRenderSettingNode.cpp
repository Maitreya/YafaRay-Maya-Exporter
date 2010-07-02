#include "bRenderSettingNode.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

const MTypeId renderSettingNode::id(0x75302);

    MObject renderSettingNode::renderRayDepth;
	MObject renderSettingNode::renderTranspShadow;
	MObject renderSettingNode::renderClayRender;
	MObject renderSettingNode::renderShadowDepth;
	MObject renderSettingNode::renderAutoThreads;
	MObject renderSettingNode::renderZBuffer;
	MObject renderSettingNode::renderThreads;

	//output settings
	MObject renderSettingNode::renderGamma;
	MObject renderSettingNode::renderGammaInput;
	MObject renderSettingNode::renderClampRGB;
	MObject renderSettingNode::renderPremultAlpha;
	MObject renderSettingNode::renderOutputMethod;//enum
	MObject renderSettingNode::renderTileOrder;//enum
	MObject renderSettingNode::renderTileSize;
	MObject renderSettingNode::renderAutoSave;
	MObject renderSettingNode::renderAlpha;
	MObject renderSettingNode::renderShowSampleMask;
	MObject renderSettingNode::renderOutputFileType;//enum
	MObject renderSettingNode::renderDrawParams;
	//MObject renderSettingNode::renderCustomString;//this is text field?

	//AA settings
	MObject renderSettingNode::renderAAPasses;
	MObject renderSettingNode::renderAASamples;
	MObject renderSettingNode::renderAAThreshold;
	MObject renderSettingNode::renderAAIncSamples;
	MObject renderSettingNode::renderFilterType;//enum
	MObject renderSettingNode::renderAAPixelWidth;

    MObject renderSettingNode::renderLightType;

	//direct lighting
	MObject renderSettingNode::renderDirCaustics;
	MObject renderSettingNode::renderPhPhotons;
	MObject renderSettingNode::renderPhCaustixMix;
	MObject renderSettingNode::renderDirCausticDepth;
	MObject renderSettingNode::renderDirCausticRadius;
	MObject renderSettingNode::renderDirAO;
	MObject renderSettingNode::renderDirAOSamples;
	MObject renderSettingNode::renderDirAODist;
	MObject renderSettingNode::renderDirAOColor;

	//pathtracing setting
	MObject renderSettingNode::renderCausticType;//enum
	//here use direct light's caustic settings
	MObject renderSettingNode::renderGIDepth;
	MObject renderSettingNode::renderUseBG;
	MObject renderSettingNode::renderGIQuality;
	MObject renderSettingNode::renderNoRecursive;

	//photon mapping settings
	//depth use pathtracing's
	//phphotons use directlight's
	MObject renderSettingNode::renderPhCausPhotons;
	MObject renderSettingNode::renderPhDiffuseRad;
	MObject renderSettingNode::renderPhCausticRad;
	MObject renderSettingNode::renderPhSearch;
	//MObject renderSettingNode::renderPhCausticMix;//nani?! this shared with directlight too
	MObject renderSettingNode::renderPhFG;
	MObject renderSettingNode::renderPhFGBounces;
	MObject renderSettingNode::renderPhFGSamples;
	MObject renderSettingNode::renderPhShowMap;

	//debug settings
	MObject renderSettingNode::renderDebugType;//enum
	MObject renderSettingNode::renderDebugMaps;

    MObject renderSettingNode::renderOutput;

void *renderSettingNode::creator()
{
		return new renderSettingNode;
}
MStatus renderSettingNode::compute(const MPlug &plug, MDataBlock &data)
{
	return MStatus::kSuccess;
}

MStatus renderSettingNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	 renderRayDepth=numAttr.create("RayDepth","rrade",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(64);

	 renderTranspShadow=numAttr.create("TransparentShadows","rtrsh",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderClayRender=numAttr.create("ClayRender","rclre",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderShadowDepth=numAttr.create("ShadowDepth","rshde",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(64);

	 renderAutoThreads=numAttr.create("AutoThreads","rauth",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderZBuffer=numAttr.create("RenderZBuffer","rrezb",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderThreads=numAttr.create("Threads","rth",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(20);

	//output settings
	 renderGamma=numAttr.create("Gamma","rga",MFnNumericData::kFloat,0.0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(5.0);

	 renderGammaInput=numAttr.create("GammaInput","rgain",MFnNumericData::kFloat,0.0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(5.0);

	 renderClampRGB=numAttr.create("ClampRGB","rclrg",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderPremultAlpha=numAttr.create("PremultAlpha","rpral",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderOutputMethod=enumAttr.create("OutputMethod","roume",0);
	 enumAttr.addField("GUI",0);
	 enumAttr.addField("Image",1);
	 enumAttr.addField("XML",2);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 renderTileOrder=enumAttr.create("TileOrder","rtior",0);
	 enumAttr.addField("Linear",0);
	 enumAttr.addField("Random",1);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 renderTileSize=numAttr.create("TileSize","rtisi",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(1024);

	 renderAutoSave=numAttr.create("AutoSave","rausa",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderAlpha=numAttr.create("AlphaOnAutosave","raoas",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderShowSampleMask=numAttr.create("ShowResampleMask","rsram",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 //renderOutputFileType;//enum//what? why didn't appear in yaf_ui.py
	 renderDrawParams=numAttr.create("DrawRenderParams","rdrp",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 //renderCustomString=numAttr.create("CustomString","rcust",MFnData::kString,"test");
	 //numAttr.setStorable(true);
	 //numAttr.setKeyable(true);
	 //numAttr.setMin(0);
	 //numAttr.setMax(50);//this is text field?

	//AA settings
	 renderAAPasses=numAttr.create("AAPasses","raapa",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(100);

	 renderAASamples=numAttr.create("AASamples","raasa",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(256);

	 renderAAThreshold=numAttr.create("AAThreshold","raath",MFnNumericData::kFloat,0.0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(1.0);

	 renderAAIncSamples=numAttr.create("AAIncSamples","raais",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(64);

	 renderFilterType=enumAttr.create("FilterType","rfity",0);//this is AA type actually....
	 enumAttr.addField("box",0);
	 enumAttr.addField("gauss",1);
	 enumAttr.addField("lanczos",2);
	 enumAttr.addField("mitchell",3);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 renderAAPixelWidth=numAttr.create("AAPixelWidth","raapw",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(20);

	 renderLightType=enumAttr.create("LightingType","rlity",0);//this is AA type actually....
	 enumAttr.addField("Direct Lighting",0);
	 enumAttr.addField("Photon Mapping",1);
	 enumAttr.addField("Bidirectional(EXPERIMENTAL)",2);
	 enumAttr.addField("Debug",3);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	//direct lighting
	 renderDirCaustics=numAttr.create("UseCaustic","rusca",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderPhPhotons=numAttr.create("Photons","rph",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(100000000);

	 renderPhCaustixMix=numAttr.create("CausticMix","rcami",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(10000);

	 renderDirCausticDepth=numAttr.create("CausticDepth","rcade",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(50);

	 renderDirCausticRadius=numAttr.create("DirCausticRadius","rcara",MFnNumericData::kDouble,0.0001);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.0001);
	 numAttr.setMax(100.0);

	 renderDirAO=numAttr.create("UseAO","rusao",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderDirAOSamples=numAttr.create("AOSamples","raosa",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(1000);

	 renderDirAODist=numAttr.create("AODistance","raodi",MFnNumericData::kFloat,0.0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.0);
	 numAttr.setMax(10000.0);

	 renderDirAOColor=numAttr.createColor("AOColor","raoco");
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setDefault(0.0,0.5,0.0);
	//pathtracing setting
	 renderCausticType=enumAttr.create("CausticTypes","rcuty",0);
	 enumAttr.addField("None",0);
	 enumAttr.addField("Path",1);
	 enumAttr.addField("Photon",2);
	 enumAttr.addField("Path+Photon",3);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	//here use direct light's caustic settings
	 renderGIDepth=numAttr.create("GIDepth","rgid",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(50);

	 renderUseBG=numAttr.create("UseBackground","ruscba",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderGIQuality=numAttr.create("PathSamples","rpasa",MFnNumericData::kInt,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0);
	 numAttr.setMax(5000);

	 renderNoRecursive=numAttr.create("NoRecursive","rnore",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	//photon mapping settings
	//depth use pathtracing's
	//phphotons use directlight's
	 renderPhCausPhotons=numAttr.create("CausticPhotons","rcaph",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(100000000);

	 renderPhDiffuseRad=numAttr.create("DiffuseRadius","rdira",MFnNumericData::kFloat,0.001);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.001);
	 numAttr.setMax(100.0);

	 renderPhCausticRad=numAttr.create("PhotonCausticRadius","rpcara",MFnNumericData::kDouble,0.0001);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(0.0001);
	 numAttr.setMax(100.0);

	 renderPhSearch=numAttr.create("Search","rse",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(10000);

	 //renderPhCausticMix;//nani?! this shared with directlight too
	 renderPhFG=numAttr.create("FinalGather","rfiga",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	 renderPhFGBounces=numAttr.create("FGBounces","rfgb",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(20);

	 renderPhFGSamples=numAttr.create("FGSamples","rfgs",MFnNumericData::kInt,1);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);
	 numAttr.setMin(1);
	 numAttr.setMax(4096);

	 renderPhShowMap=numAttr.create("ShowMap","rshma",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	//debug settings
	 renderDebugType=enumAttr.create("DebugType","rdety",0);
	 enumAttr.addField("N",0);
	 enumAttr.addField("dpdU",1);
	 enumAttr.addField("dpdV",2);
	 enumAttr.addField("NU",3);
	 enumAttr.addField("NV",4);
	 enumAttr.addField("dsdU",5);
	 enumAttr.addField("dsdV",6);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 renderDebugMaps=numAttr.create("PerturbedNormals","rpeno",MFnNumericData::kBoolean,0);
	 numAttr.setStorable(true);
	 numAttr.setKeyable(true);

	//an output
	 renderOutput=numAttr.create("WorldSettingOutput","wwso",MFnNumericData::kBoolean,1);
	 numAttr.setHidden(true);

	 setAttribute();


	return stat;

}
void renderSettingNode::setAttribute()
{
	addAttribute(renderRayDepth);
	addAttribute(renderTranspShadow);
	addAttribute(renderClayRender);
	addAttribute(renderShadowDepth);
	addAttribute(renderAutoThreads);
	addAttribute(renderZBuffer);
	addAttribute(renderThreads);

	//output settings
	addAttribute(renderGamma);
	addAttribute(renderGammaInput);
	addAttribute(renderClampRGB);
	addAttribute(renderPremultAlpha);
	addAttribute(renderOutputMethod);//enum
	addAttribute(renderTileOrder);//enum
	addAttribute(renderTileSize);
	addAttribute(renderAutoSave);
	addAttribute(renderAlpha);
	addAttribute(renderShowSampleMask);
	addAttribute(renderOutputFileType);//enum
	addAttribute(renderDrawParams);
	//addAttribute(renderCustomString);//this is text field?

	//AA settings
	addAttribute(renderAAPasses);
	addAttribute(renderAASamples);
	addAttribute(renderAAThreshold);
	addAttribute(renderAAIncSamples);
	addAttribute(renderFilterType);//enum
	addAttribute(renderAAPixelWidth);

    addAttribute(renderLightType);
	//direct lighting
	addAttribute(renderDirCaustics);
	addAttribute(renderPhPhotons);
	addAttribute(renderPhCaustixMix);
	addAttribute(renderDirCausticDepth);
	addAttribute(renderDirCausticRadius);
	addAttribute(renderDirAO);
	addAttribute(renderDirAOSamples);
	addAttribute(renderDirAODist);
	addAttribute(renderDirAOColor);

	//pathtracing setting
	addAttribute(renderCausticType);//enum
	//here use direct light's caustic settings
	addAttribute(renderGIDepth);
	addAttribute(renderUseBG);
	addAttribute(renderGIQuality);
	addAttribute(renderNoRecursive);

	//photon mapping settings
	//depth use pathtracing's
	//phphotons use directlight's
	addAttribute(renderPhCausPhotons);
	addAttribute(renderPhDiffuseRad);
	addAttribute(renderPhCausticRad);
	addAttribute(renderPhSearch);
	//addAttribute(renderPhCausticMix);//nani?! this shared with directlight too
	addAttribute(renderPhFG);
	addAttribute(renderPhFGBounces);
	addAttribute(renderPhFGSamples);
	addAttribute(renderPhShowMap);

	addAttribute(renderDebugType);
	addAttribute(renderDebugMaps);

	addAttribute(renderOutput);

	attributeAffects(renderRayDepth,renderOutput);
	attributeAffects(renderTranspShadow,renderOutput);
	attributeAffects(renderClayRender,renderOutput);
	attributeAffects(renderShadowDepth,renderOutput);
	attributeAffects(renderAutoThreads,renderOutput);
	attributeAffects(renderZBuffer,renderOutput);
	attributeAffects(renderThreads,renderOutput);

	//output settings
	attributeAffects(renderGamma,renderOutput);
	attributeAffects(renderGammaInput,renderOutput);
	attributeAffects(renderClampRGB,renderOutput);
	attributeAffects(renderPremultAlpha,renderOutput);
	attributeAffects(renderOutputMethod,renderOutput);//enum
	attributeAffects(renderTileOrder,renderOutput);//enum
	attributeAffects(renderTileSize,renderOutput);
	attributeAffects(renderAutoSave,renderOutput);
	attributeAffects(renderAlpha,renderOutput);
	attributeAffects(renderShowSampleMask,renderOutput);
	attributeAffects(renderOutputFileType,renderOutput);//enum
	attributeAffects(renderDrawParams,renderOutput);
	//attributeAffects(renderCustomString,renderOutput);//this is text field?

	//AA settings
	attributeAffects(renderAAPasses,renderOutput);
	attributeAffects(renderAASamples,renderOutput);
	attributeAffects(renderAAThreshold,renderOutput);
	attributeAffects(renderAAIncSamples,renderOutput);
	attributeAffects(renderFilterType,renderOutput);//enum
	attributeAffects(renderAAPixelWidth,renderOutput);

    attributeAffects(renderLightType,renderOutput);
	//direct lighting
	attributeAffects(renderDirCaustics,renderOutput);
	attributeAffects(renderPhPhotons,renderOutput);
	attributeAffects(renderPhCaustixMix,renderOutput);
	attributeAffects(renderDirCausticDepth,renderOutput);
	attributeAffects(renderDirCausticRadius,renderOutput);
	attributeAffects(renderDirAO,renderOutput);
	attributeAffects(renderDirAOSamples,renderOutput);
	attributeAffects(renderDirAODist,renderOutput);
	attributeAffects(renderDirAOColor,renderOutput);

	//pathtracing setting
	attributeAffects(renderCausticType,renderOutput);//enum
	//here use direct light's caustic settings
	attributeAffects(renderGIDepth,renderOutput);
	attributeAffects(renderUseBG,renderOutput);
	attributeAffects(renderGIQuality,renderOutput);
	attributeAffects(renderNoRecursive,renderOutput);

	//photon mapping settings
	//depth use pathtracing's
	//phphotons use directlight's
	attributeAffects(renderPhCausPhotons,renderOutput);
	attributeAffects(renderPhDiffuseRad,renderOutput);
	attributeAffects(renderPhCausticRad,renderOutput);
	attributeAffects(renderPhSearch,renderOutput);
	//attributeAffects(renderPhCausticMix,renderOutput);//nani?! this shared with directlight too
	attributeAffects(renderPhFG,renderOutput);
	attributeAffects(renderPhFGBounces,renderOutput);
	attributeAffects(renderPhFGSamples,renderOutput);
	attributeAffects(renderPhShowMap,renderOutput);

	attributeAffects(renderDebugType,renderOutput);
	attributeAffects(renderDebugMaps,renderOutput);


}

