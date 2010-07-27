#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bReadShader.h"

#include<maya/MString.h>
#include<maya/MGlobal.h>
#include<maya/MStringArray.h>
#include<maya/MSelectionList.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MPlug.h>
#include<maya/MFnNumericData.h>
#include<interface/yafrayinterface.h>
#include<map>
#include<string>
using namespace yafaray;
using namespace std;

MStatus getShader::readShader ( yafrayInterface_t &yI , std::map<string , yafaray::material_t*> &materialMap)
{
	MStatus stat=MStatus::kSuccess;

	//get all the glass materials
	MString getGlassCmd("ls -type yafGlass");
	MStringArray getGlassResult;
	MGlobal::executeCommand(getGlassCmd,getGlassResult);

	for(unsigned int i=0;i<getGlassResult.length();i++)
	{
		MSelectionList list;
		MGlobal::getSelectionListByName(getGlassResult[i],list);
		for(unsigned int index=0;index<list.length();index++)
		{
			MObject GlassNode;
			list.getDependNode(index,GlassNode);
			MFnDependencyNode GlassFn;
			GlassFn.setObject(GlassNode);

			yI.paramsClearAll();
			yI.paramsSetString("type","glass");

			MObject absorbColor;
			GlassFn.findPlug("AbsorbColor").getValue(absorbColor);
			MFnNumericData absorbData(absorbColor);
			float absorbR,absorbG,absorbB;
			absorbData.getData(absorbR,absorbG,absorbB);
			yI.paramsSetColor("absorption",absorbR,absorbG,absorbB);

			float absorbDist;
			GlassFn.findPlug("AbsorbDistance").getValue(absorbDist);
			yI.paramsSetFloat("absorption_dist",absorbDist);

			MObject filterColor;
			GlassFn.findPlug("FilterColor").getValue(filterColor);
			MFnNumericData filterData(filterColor);
			float filterR, filterG,filterB;
			filterData.getData(filterR,filterG,filterB);
			yI.paramsSetColor("filter_color",filterR,filterG,filterB);

			MObject mirrorColor;
			GlassFn.findPlug("MirrorColor").getValue(mirrorColor);
			MFnNumericData mirrorData(mirrorColor);
			float mirrorR,mirrorG,mirrorB;
			mirrorData.getData(mirrorR,mirrorG,mirrorB);
			yI.paramsSetColor("mirror_color",mirrorR,mirrorG,mirrorB);

			float ior;
			GlassFn.findPlug("IOR").getValue(ior);
			yI.paramsSetFloat("IOR",ior);

			float transmitFilter;
			GlassFn.findPlug("TransmitFilter").getValue(transmitFilter);
			yI.paramsSetFloat("transmit_filter",transmitFilter);

			float dispPower;
			GlassFn.findPlug("DispersionPower").getValue(dispPower);
			yI.paramsSetFloat("dispersion_power",dispPower);

			bool fakeShadows;
			GlassFn.findPlug("FakeShadows").getValue(fakeShadows);
			yI.paramsSetBool("fake_shadows",fakeShadows);

			yI.paramsEndList();

			//seems this doesn't work
			yafaray::material_t* glassMat=yI.createMaterial(GlassFn.name().asChar());
			const char * glassName=GlassFn.name().asChar();
			string gName(glassName);
			//test output
			cout<<"================materials=============="<<endl;
			cout<<gName<<endl;
			materialMap[gName]=glassMat;
		}
	}

	//then deal with other materials, but not implemented yet
	//coated glossy node
	MString getCoatedGlossyCmd("ls -type yafCoatedGlossy");
	MStringArray getCoatedGlossyResult;
	MGlobal::executeCommand(getCoatedGlossyCmd,getCoatedGlossyResult);

	for (unsigned int i=0; i<getCoatedGlossyResult.length(); i++)
	{
		MSelectionList list;
		MGlobal::getSelectionListByName(getCoatedGlossyResult[i],list);
		for(unsigned int index=0;index<list.length();index++)
		{
			MObject CoatedGlossyNode;
			list.getDependNode(index,CoatedGlossyNode);
			MFnDependencyNode CoatedGlossyFn;
			CoatedGlossyFn.setObject(CoatedGlossyNode);

			yI.paramsClearAll();
			yI.paramsSetString("type","coated_glossy");

			MObject diffuseColor;
			CoatedGlossyFn.findPlug("DiffuseColor").getValue(diffuseColor);
			MFnNumericData diffuseData(diffuseColor);
			float diffuseR,diffuseG,diffuseB;
			diffuseData.getData(diffuseR, diffuseG, diffuseB);
			yI.paramsSetColor("diffuse_color",diffuseR, diffuseG, diffuseB);

			float diffuseReflect;
			CoatedGlossyFn.findPlug("DiffuseReflection").getValue(diffuseReflect);
			yI.paramsSetFloat("diffuse_reflect",diffuseReflect);

			MObject glossyColor;
			CoatedGlossyFn.findPlug("GlossyColor").getValue(glossyColor);
			MFnNumericData glossyData(glossyColor);
			float glossyR,glossyG,glossyB;
			glossyData.getData(glossyR,glossyG,glossyB);
			yI.paramsSetColor("color",glossyR,glossyG,glossyB);

			float glossyReflect;
			CoatedGlossyFn.findPlug("GlossyReflection").getValue(glossyReflect);
			yI.paramsSetFloat("glossy_reflect",glossyReflect);

			float exponent;
			CoatedGlossyFn.findPlug("Exponent").getValue(exponent);
			yI.paramsSetFloat("exponent",exponent);

			bool asDiffuse;
			CoatedGlossyFn.findPlug("AsDiffuse").getValue(asDiffuse);
			yI.paramsSetBool("as_diffuse",asDiffuse);

			bool anisotropic;
			CoatedGlossyFn.findPlug("Anisotropic").getValue(anisotropic);
			yI.paramsSetBool("anisotropic",anisotropic);

			float expHorizontal;
			CoatedGlossyFn.findPlug("ExponentHorizontal").getValue(expHorizontal);
			yI.paramsSetFloat("exp_u",expHorizontal);

			float expVertical;
			CoatedGlossyFn.findPlug("ExponentVertical").getValue(expVertical);
			yI.paramsSetFloat("exp_v",expVertical);

			float ior;
			CoatedGlossyFn.findPlug("IOR").getValue(ior);
			yI.paramsSetFloat("IOR",ior);

			yI.paramsEndList();

			const char* coatedGlossyName=CoatedGlossyFn.name().asChar();
			yafaray::material_t* coatedGlossyMat=yI.createMaterial(coatedGlossyName);
			string coatedName(coatedGlossyName);
			cout<<coatedName<<endl;
			materialMap[coatedName]=coatedGlossyMat;
		}
	}

	//shinydiffuse
	MString getShinyDiffuseCmd("ls -type yafShinyDiffuse");
	MStringArray getShinyDiffuseResult;
	MGlobal::executeCommand(getShinyDiffuseCmd,getShinyDiffuseResult);
	for (unsigned int i=0; i<getShinyDiffuseResult.length(); i++)
	{
		MSelectionList list;
		MGlobal::getSelectionListByName(getShinyDiffuseResult[i],list);
		for(unsigned int index=0;index<list.length();index++)
		{
			MObject ShinyDiffuseNode;
			list.getDependNode(index,ShinyDiffuseNode);
			MFnDependencyNode ShinyDiffuseFn;
			ShinyDiffuseFn.setObject(ShinyDiffuseNode);

			yI.paramsClearAll();
			yI.paramsSetString("type","shinydiffusemat");

			MObject Color;
			ShinyDiffuseFn.findPlug("Color").getValue(Color);
			MFnNumericData colorData(Color);
			float colorR,colorG,colorB;
			colorData.getData(colorR,colorG,colorB);
			yI.paramsSetColor("color",colorR,colorG,colorB);

			MObject mirrorColor;
			ShinyDiffuseFn.findPlug("MirrorColor").getValue(mirrorColor);
			MFnNumericData mirrorData(mirrorColor);
			float mirrorR,mirrorG,mirrorB;
			mirrorData.getData(mirrorR,mirrorG,mirrorB);
			yI.paramsSetColor("mirror_color",mirrorR,mirrorG,mirrorB);

			float diffuseReflection;
			ShinyDiffuseFn.findPlug("DiffuseReflection").getValue(diffuseReflection);
			yI.paramsSetFloat("diffuse_reflect",diffuseReflection);

			float mirrorStrength;
			ShinyDiffuseFn.findPlug("MirrorStrength").getValue(mirrorStrength);
			yI.paramsSetFloat("specular_reflect",mirrorStrength);

			float transparency;
			ShinyDiffuseFn.findPlug("Transparency").getValue(transparency);
			yI.paramsSetFloat("transparency",transparency);

			float translucency;
			ShinyDiffuseFn.findPlug("Translucency").getValue(translucency);
			yI.paramsSetFloat("translucency",translucency);

			float transmitFilter;
			ShinyDiffuseFn.findPlug("TransmitFilter").getValue(transmitFilter);
			yI.paramsSetFloat("transmit_filter",transmitFilter);

			float emit;
			ShinyDiffuseFn.findPlug("Emit").getValue(emit);
			yI.paramsSetFloat("emit",emit);

			bool fresnel;
			ShinyDiffuseFn.findPlug("Fresnel").getValue(fresnel);
			yI.paramsSetBool("fresnel_effect",fresnel);

			float ior;
			ShinyDiffuseFn.findPlug("IOR").getValue(ior);
			yI.paramsSetFloat("IOR",ior);

			yI.paramsEndList();

			const char* shinyDiffuseName=ShinyDiffuseFn.name().asChar();
			yafaray::material_t* shinyDiffuseMat=yI.createMaterial(shinyDiffuseName);
			string shinyName(shinyDiffuseName);
			cout<<shinyName<<endl;
			materialMap[shinyName]=shinyDiffuseMat;

		}
	}


	return stat;
}