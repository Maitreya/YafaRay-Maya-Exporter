#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bReadRender.h"

#include <maya/MString.h>
#include<maya/MStringArray.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericData.h>
#include <interface/yafrayinterface.h>
#include <maya/MPlug.h>
using namespace yafaray;

MStatus getRender::readRender(yafaray::yafrayInterface_t &yI)
{
	MStatus stat;
	MString listRnderSetting("ls -type yafRenderSetting");
	MStringArray listRenderResult;
	MGlobal::executeCommand(listRnderSetting,listRenderResult);

	MSelectionList list;
	MGlobal::getSelectionListByName(listRenderResult[0],list);
	for(unsigned int index=0; index<list.length(); index++)
	{
		MObject renderSettingNode;
		list.getDependNode(index, renderSettingNode);
		MFnDependencyNode renderFn(renderSettingNode);

		readOutput(yI,renderFn);
		readIntegrator(yI,renderFn);
//		readAA(yI,renderFn);


	}


	return stat;

}
MStatus getRender::readOutput(yafrayInterface_t &yI, MFnDependencyNode &renderFn)
{
	MStatus stat;
	float gammaInput;
	//damn.....why this guy is separated here?
	renderFn.findPlug("GammaInput").getValue(gammaInput);
	yI.setInputGamma(gammaInput,true);

	return stat;
}
MStatus getRender::readIntegrator(yafaray::yafrayInterface_t &yI, MFnDependencyNode &renderFn)
{
	MStatus stat;
	
	yI.paramsClearAll();

	//these are general settings
	int rayDepth;
	renderFn.findPlug("RayDepth").getValue(rayDepth);
	//test output
	cout<<rayDepth<<endl;
	yI.paramsSetInt("raydepth",rayDepth);

	int shadowDepth;
	renderFn.findPlug("ShadowDepth").getValue(shadowDepth);
	yI.paramsSetInt("shadowDepth",shadowDepth);

	bool transShadow;
	renderFn.findPlug("TransparentShadows").getValue(transShadow);
	yI.paramsSetBool("transPShad", transShadow);

	short lightingType;
	renderFn.findPlug("LightingType").getValue(lightingType);
	//direct lighting set
	if (lightingType==0)
	{
		yI.paramsSetString("type","directlighting");

		bool useCaustics;
		renderFn.findPlug("UseCaustic").getValue(useCaustics);
		yI.paramsSetBool("caustics",useCaustics);

		if (useCaustics)
		{
			int photons;
			renderFn.findPlug("Photons").getValue(photons);
			yI.paramsSetInt("photons",photons);

			int causticMix;
			renderFn.findPlug("CausticMix").getValue(causticMix);
			yI.paramsSetInt("caustic_mix",causticMix);

			int causticDepth;
			renderFn.findPlug("CausticDepth").getValue(causticDepth);
			yI.paramsSetInt("caustic_depth",causticDepth);

			float causticRadius;
			renderFn.findPlug("DirCausticRadius").getValue(causticRadius);
			yI.paramsSetFloat("caustic_radius",causticRadius);
		}

		bool useAO;
		renderFn.findPlug("UseAO").getValue(useAO);
		if (useAO)
		{
			yI.paramsSetBool("do_AO",useAO);
			
			int aoSample;
			renderFn.findPlug("AOSamples").getValue(aoSample);
			yI.paramsSetInt("AO_samples",aoSample);

			float aoDistance;
			renderFn.findPlug("AODistance").getValue(aoDistance);
			yI.paramsSetFloat("AO_distance",aoDistance);

			MObject aoColor;
			renderFn.findPlug("AOColor").getValue(aoColor);
			MFnNumericData aocData(aoColor);
			float aocR,aocG,aocB;
			aocData.getData(aocR,aocG,aocB);
			yI.paramsSetColor("AO_color",aocR,aocG,aocB);
		}
	}
	else if (lightingType==1)
	{
		//path tracing settings
	}
	else if (lightingType==2)
	{
		//photon mapping settings
	}
	else if (lightingType==3)
	{
		//debug settings
	}

	yI.createIntegrator("default");

	return stat;
}

MStatus getRender::readAA(yafaray::yafrayInterface_t &yI, MFnDependencyNode &renderFn)
{
	MStatus stat;
	yI.paramsClearAll();
	//yI.paramsSetString("camera_name","cam");
	//yI.paramsSetString("integrator_name","default");
	//yI.paramsSetString("volintegrator_name","volintegr");
	//yI.paramsSetString("background_name","world_background");

	float gamma;
	renderFn.findPlug("Gamma").getValue(gamma);
	yI.paramsSetFloat("gamma",gamma);

	int aaPasses;
	renderFn.findPlug("AAPasses").getValue(aaPasses);
	yI.paramsSetInt("AA_passes",aaPasses);

	int aaSamples;
	renderFn.findPlug("AASamples").getValue(aaSamples);
	yI.paramsSetInt("AA_minsamples",aaSamples);

	int aaIncSamples;
	renderFn.findPlug("AAIncSamples").getValue(aaIncSamples);
	yI.paramsSetInt("AA_inc_samples",aaIncSamples);

	float aaPixelWidth;
	renderFn.findPlug("AAPixelWidth").getValue(aaPixelWidth);
	yI.paramsSetFloat("AA_pixelwidth",aaPixelWidth);

	float aaThreshold;
	renderFn.findPlug("AAThreshold").getValue(aaThreshold);
	yI.paramsSetFloat("AA_threshold",aaThreshold);

	short filterType;
	renderFn.findPlug("FilterType").getValue(filterType);
	switch (filterType)
	{
	case 0:
		yI.paramsSetString("filter_type","box");
		break;
	case 1:
		yI.paramsSetString("filter_type","gauss");
		break;
	case 2:
		yI.paramsSetString("filter_type","lanczos");
		break;
	case 3:
		yI.paramsSetString("filter_type","mitchell");
		break;
	default:
		break;
	}
	//i don't know what's these things: xstart, ystart.....
	//don't know how to get the size of the rendering pic now.......
	//yI.paramsSetInt("xstart",0);
	//yI.paramsSetInt("ystart",0);

	/*yI.paramsSetInt("width",640);
	yI.paramsSetInt("height",480);
	yI.paramsSetBool("z_channel",false);*/

	bool clampRGB;
	renderFn.findPlug("ClampRGB").getValue(clampRGB);
	yI.paramsSetBool("clamp_rgb",clampRGB);

	//i am not sure if the two is the same thing, already messy about these names....
	bool showSampleMask;
	renderFn.findPlug("ShowResampleMask").getValue(showSampleMask);
	yI.paramsSetBool("show_sam_pix",showSampleMask);

	int tileSize;
	renderFn.findPlug("TileSize").getValue(tileSize);
	yI.paramsSetInt("tile_size",tileSize);

	bool premult;
	renderFn.findPlug("PremultAlpha").getValue(premult);
	yI.paramsSetBool("premult",premult);

	short tileOrder;
	renderFn.findPlug("TileOrder").getValue(tileOrder);
	if (tileOrder==0)
	{
		yI.paramsSetString("tiles_order","linear");
	}
	else
	{
		yI.paramsSetString("tiles_order","random");
	}

	bool zChannel;
	renderFn.findPlug("RenderZBuffer").getValue(zChannel);
	yI.paramsSetBool("z_channel",zChannel);

	bool drawParams;
	renderFn.findPlug("DrawRenderParams").getValue(drawParams);
	yI.paramsSetBool("drawParams",drawParams);

	//don't know how to use this yet......
	yI.paramsSetString("customString","this is custom string");

	bool autoThreads;
	renderFn.findPlug("AutoThreads").getValue(autoThreads);
	if (autoThreads)
	{
		yI.paramsSetInt("threads",-1);
	}
	else
	{
		int threads;
		renderFn.findPlug("Threads").getValue(threads);
		yI.paramsSetInt("threads",threads);
	}





	return stat;
}