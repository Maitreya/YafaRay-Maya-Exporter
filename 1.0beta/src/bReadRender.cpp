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

MStatus getRender::createRender(yafaray::yafrayInterface_t &yI)
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

		setGammaInput(yI,renderFn);
		readIntegrator(yI,renderFn);
		readRender(yI,renderFn);
		readOutput(yI,renderFn);


	}


	return stat;

}
MStatus getRender::setGammaInput(yafrayInterface_t &yI, MFnDependencyNode &renderFn)
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
		yI.paramsSetString("type","pathtracing");

		//GI Depth
		int bounces;
		renderFn.findPlug("GIDepth").getValue(bounces);
		yI.paramsSetInt("bounces",bounces);

		int pathSamples;
		renderFn.findPlug("PathSamples").getValue(pathSamples);
		yI.paramsSetInt("path_samples",pathSamples);

		bool noRecursive;
		renderFn.findPlug("NoRecursive").getValue(noRecursive);
		yI.paramsSetBool("no_recursive",noRecursive);

		short causType;
		renderFn.findPlug("CausticTypes").getValue(causType);
		switch(causType)
		{
		case 0:
			yI.paramsSetString("caustic_type","none");
			break;
		case 1:
			yI.paramsSetString("caustic_type","path");
			break;
		case 2:
			yI.paramsSetString("caustic_type","photon");
			break;
		case 3:
			yI.paramsSetString("caustic_type","both");
			break;
		default:
		    break;	
		}
		if (causType==2||causType==3)
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
	}
	else if (lightingType==2)
	{
		//photon mapping settings
		yI.paramsSetString("type","photonmapping");

		int bounces;
		renderFn.findPlug("GIDepth").getValue(bounces);
		yI.paramsSetInt("bounces",bounces);

		int photons;
		renderFn.findPlug("Photons").getValue(photons);
		yI.paramsSetInt("photons",photons);

		float diffuseRadius;
		renderFn.findPlug("DiffuseRadius").getValue(diffuseRadius);
		yI.paramsSetFloat("diffuseRadius",diffuseRadius);

		int causticPhotons;
		renderFn.findPlug("CausticPhotons").getValue(causticPhotons);
		yI.paramsSetInt("cPhotons",causticPhotons);

		float causticRadius;
		renderFn.findPlug("PhotonCausticRadius").getValue(causticRadius);
		yI.paramsSetFloat("causticRadius",causticRadius);

		int causticMix;
		renderFn.findPlug("CausticMix").getValue(causticMix);
		yI.paramsSetInt("caustic_mix",causticMix);

		int search;
		renderFn.findPlug("Search").getValue(search);
		yI.paramsSetInt("search",search);

		bool finalGather;
		renderFn.findPlug("FinalGather").getValue(finalGather);
		yI.paramsSetBool("finalGather",finalGather);

		int fgBounces;
		renderFn.findPlug("FGBounces").getValue(fgBounces);
		yI.paramsSetInt("fg_bounces",fgBounces);

		int fgSamples;
		renderFn.findPlug("FGSamples").getValue(fgSamples);
		yI.paramsSetInt("fg_samples",fgSamples);

		bool showMap;
		renderFn.findPlug("ShowMap").getValue(showMap);
		yI.paramsSetBool("show_map",showMap);


	}
	else if (lightingType==3)
	{
		//debug settings
	}

	yI.createIntegrator("default");

	return stat;
}

MStatus getRender::readRender(yafaray::yafrayInterface_t &yI, MFnDependencyNode &renderFn)
{
	MStatus stat;
	yI.paramsClearAll();
	yI.paramsSetString("camera_name","cam");
	yI.paramsSetString("integrator_name","default");
	yI.paramsSetString("volintegrator_name","volintegr");
	yI.paramsSetString("background_name","world_background");

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
	//i don't know what's these things: xstart, ystart.....
	//don't know how to get the size of the rendering pic now.......
	//yI.paramsSetInt("xstart",0);
	//yI.paramsSetInt("ystart",0);


	return stat;
}
MStatus getRender::readOutput(yafrayInterface_t &yI, MFnDependencyNode &renderFn)
{
	MStatus stat=MStatus::kSuccess;

	int width;
	renderFn.findPlug("width").getValue(width);
	yI.paramsSetInt("width",width);

	int height;
	renderFn.findPlug("height").getValue(height);
	yI.paramsSetInt("height",height);

	bool zChannel;
	renderFn.findPlug("RenderZBuffer").getValue(zChannel);
	yI.paramsSetBool("z_channel",zChannel);

	return stat;
}

MStatus getRender::getImageWidth(int &width)
{
	MStatus stat=MStatus::kSuccess;
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

		renderFn.findPlug("width").getValue(width);

	}

	return stat;
}

MStatus getRender::getImageHeight(int &height)
{
	MStatus stat=MStatus::kSuccess;

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

		renderFn.findPlug("height").getValue(height);

	}

	return stat;
}
MStatus getRender::getAnimationInfo(int &start, int &end, MString &format)
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

		renderFn.findPlug("StartFrame").getValue(start);
		renderFn.findPlug("EndFrame").getValue(end);
		short fileFormat;
		renderFn.findPlug("OutputFileType").getValue(fileFormat);
		switch (fileFormat)
		{
		case 0:
			format="tiff";
			break;
		case 1:
			format="tga";
			break;
		case 2:
			format="jpg";
			break;
		}

	}


	return stat;
}