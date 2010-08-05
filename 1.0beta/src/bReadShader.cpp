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

	if (getGlassResult.length()>0)
	{
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
					cout<<"================glass material=============="<<endl;
					cout<<gName<<endl;
					materialMap[gName]=glassMat;
				}
			}
	}

	MString getRoughGlassCmd("ls -type yafRoughGlass");
	MStringArray getRoughGlassResult;
	MGlobal::executeCommand(getRoughGlassCmd,getRoughGlassResult);

	if (getRoughGlassResult.length()>0)
	{
		for(unsigned int i=0;i<getRoughGlassResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getRoughGlassResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject roughGlassNode;
				list.getDependNode(index,roughGlassNode);
				MFnDependencyNode roughGlassFn;
				roughGlassFn.setObject(roughGlassNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","rough_glass");

				MObject roughGlassColors;
				float colorsR,colorsG,colorsB;
				float roughGlassFloats;

				roughGlassFn.findPlug("Exponent").getValue(roughGlassFloats);
				yI.paramsSetFloat("exponent",roughGlassFloats);

				roughGlassFn.findPlug("Roughtness").getValue(roughGlassFloats);
				yI.paramsSetFloat("alpha",roughGlassFloats);

				roughGlassFn.findPlug("AbsorbColor").getValue(roughGlassColors);
				MFnNumericData absorbData(roughGlassColors);
				absorbData.getData(colorsR,colorsG,colorsB);
				yI.paramsSetColor("absorption",colorsR,colorsG,colorsB);

				roughGlassFn.findPlug("AbsorbDistance").getValue(roughGlassFloats);
				yI.paramsSetFloat("absorption_dist",roughGlassFloats);

				roughGlassFn.findPlug("FilterColor").getValue(roughGlassColors);
				MFnNumericData filterData(roughGlassColors);
				filterData.getData(colorsR,colorsG,colorsB);
				yI.paramsSetColor("filter_color",colorsR,colorsG,colorsB);

				roughGlassFn.findPlug("MirrorColor").getValue(roughGlassColors);
				MFnNumericData mirrorData(roughGlassColors);
				mirrorData.getData(colorsR,colorsG,colorsB);
				yI.paramsSetColor("mirror_color",colorsR,colorsG,colorsB);

				roughGlassFn.findPlug("IOR").getValue(roughGlassFloats);
				yI.paramsSetFloat("IOR",roughGlassFloats);

				roughGlassFn.findPlug("TransmitFilter").getValue(roughGlassFloats);
				yI.paramsSetFloat("transmit_filter",roughGlassFloats);

				roughGlassFn.findPlug("DispersionPower").getValue(roughGlassFloats);
				yI.paramsSetFloat("dispersion_power",roughGlassFloats);

				bool fakeShadows;
				roughGlassFn.findPlug("FakeShadows").getValue(fakeShadows);
				yI.paramsSetBool("fake_shadows",fakeShadows);

				yI.paramsEndList();

				yafaray::material_t* roughGlassMat=yI.createMaterial(roughGlassFn.name().asChar());
				const char * roughName=roughGlassFn.name().asChar();
				string rName(roughName);
				//test output
				cout<<"================rough glass materials=============="<<endl;
				cout<<rName<<endl;
				materialMap[rName]=roughGlassMat;
			}
		}
	}

	//then deal with other materials, but not implemented yet
	//coated glossy node
	MString getCoatedGlossyCmd("ls -type yafCoatedGlossy");
	MStringArray getCoatedGlossyResult;
	MGlobal::executeCommand(getCoatedGlossyCmd,getCoatedGlossyResult);

	if(getCoatedGlossyResult.length()>0)
	{
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

					MObject coatedColors;
					float coatedR,coatedG,coatedB;
					float coatedFloats;

					yI.paramsClearAll();
					yI.paramsSetString("type","coated_glossy");

					CoatedGlossyFn.findPlug("DiffuseColor").getValue(coatedColors);
					MFnNumericData diffuseData(coatedColors);
					diffuseData.getData(coatedR,coatedG,coatedB);
					yI.paramsSetColor("diffuse_color",coatedR,coatedG,coatedB);

					CoatedGlossyFn.findPlug("DiffuseReflection").getValue(coatedFloats);
					yI.paramsSetFloat("diffuse_reflect",coatedFloats);

					CoatedGlossyFn.findPlug("GlossyColor").getValue(coatedColors);
					MFnNumericData glossyData(coatedColors);
					glossyData.getData(coatedR,coatedG,coatedB);
					yI.paramsSetColor("color",coatedR,coatedG,coatedB);

					CoatedGlossyFn.findPlug("GlossyReflection").getValue(coatedFloats);
					yI.paramsSetFloat("glossy_reflect",coatedFloats);

					CoatedGlossyFn.findPlug("Exponent").getValue(coatedFloats);
					yI.paramsSetFloat("exponent",coatedFloats);

					bool asDiffuse;
					CoatedGlossyFn.findPlug("AsDiffuse").getValue(asDiffuse);
					yI.paramsSetBool("as_diffuse",asDiffuse);

					bool anisotropic;
					CoatedGlossyFn.findPlug("Anisotropic").getValue(anisotropic);
					yI.paramsSetBool("anisotropic",anisotropic);

					CoatedGlossyFn.findPlug("ExponentHorizontal").getValue(coatedFloats);
					yI.paramsSetFloat("exp_u",coatedFloats);

					CoatedGlossyFn.findPlug("ExponentVertical").getValue(coatedFloats);
					yI.paramsSetFloat("exp_v",coatedFloats);

					CoatedGlossyFn.findPlug("IOR").getValue(coatedFloats);
					yI.paramsSetFloat("IOR",coatedFloats);

					yI.paramsEndList();

					const char* coatedGlossyName=CoatedGlossyFn.name().asChar();
					yafaray::material_t* coatedGlossyMat=yI.createMaterial(coatedGlossyName);
					string coatedName(coatedGlossyName);
					cout<<"================coated glossy materials=============="<<endl;
					cout<<coatedName<<endl;
					materialMap[coatedName]=coatedGlossyMat;
				}
			}
	}

	MString getGlossyCmd("ls -type yafGlossy");
	MStringArray getGlossyResult;
	MGlobal::executeCommand(getGlossyCmd,getGlossyResult);

	if(getGlossyResult.length()>0)
	{
		for (unsigned int i=0; i<getGlossyResult.length(); i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getGlossyResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject GlossyNode;
				list.getDependNode(index,GlossyNode);
				MFnDependencyNode GlossyFn;
				GlossyFn.setObject(GlossyNode);

				MObject glossyColors;
				float glossyR,glossyG,glossyB;
				float glossyFloats;

				yI.paramsClearAll();
				yI.paramsSetString("type","coated_glossy");

				GlossyFn.findPlug("DiffuseColor").getValue(glossyColors);
				MFnNumericData diffuseData(glossyColors);
				diffuseData.getData(glossyR,glossyG,glossyB);
				yI.paramsSetColor("diffuse_color",glossyR,glossyG,glossyB);

				GlossyFn.findPlug("DiffuseReflection").getValue(glossyFloats);
				yI.paramsSetFloat("diffuse_reflect",glossyFloats);

				GlossyFn.findPlug("GlossyColor").getValue(glossyColors);
				MFnNumericData glossyData(glossyColors);
				glossyData.getData(glossyR,glossyG,glossyB);
				yI.paramsSetColor("color",glossyR,glossyG,glossyB);

				GlossyFn.findPlug("GlossyReflection").getValue(glossyFloats);
				yI.paramsSetFloat("glossy_reflect",glossyFloats);

				GlossyFn.findPlug("Exponent").getValue(glossyFloats);
				yI.paramsSetFloat("exponent",glossyFloats);

				bool asDiffuse;
				GlossyFn.findPlug("AsDiffuse").getValue(asDiffuse);
				yI.paramsSetBool("as_diffuse",asDiffuse);

				bool anisotropic;
				GlossyFn.findPlug("Anisotropic").getValue(anisotropic);
				yI.paramsSetBool("anisotropic",anisotropic);

				GlossyFn.findPlug("ExponentHorizontal").getValue(glossyFloats);
				yI.paramsSetFloat("exp_u",glossyFloats);

				GlossyFn.findPlug("ExponentVertical").getValue(glossyFloats);
				yI.paramsSetFloat("exp_v",glossyFloats);

				yI.paramsEndList();

				const char* glossyName=GlossyFn.name().asChar();
				yafaray::material_t* glossyMat=yI.createMaterial(glossyName);
				string gName(glossyName);
				cout<<"================glossy materials=============="<<endl;
				cout<<gName<<endl;
				materialMap[gName]=glossyMat;
			}
		}
	}

	//shinydiffuse
	MString getShinyDiffuseCmd("ls -type yafShinyDiffuse");
	MStringArray getShinyDiffuseResult;
	MGlobal::executeCommand(getShinyDiffuseCmd,getShinyDiffuseResult);
	if (getShinyDiffuseResult.length()>0)
	{
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
					cout<<"================shiny diffuse materials=============="<<endl;
					cout<<shinyName<<endl;
					materialMap[shinyName]=shinyDiffuseMat;

				}
			}
	}

	MString getBlendCmd("ls -type yafBlend");
	MStringArray getBlendResult;
	MGlobal::executeCommand(getBlendCmd,getBlendResult);
	if (getBlendResult.length()>0)
	{
			for (unsigned int i=0; i<getBlendResult.length(); i++)
			{
				MSelectionList list;
				MGlobal::getSelectionListByName(getBlendResult[i],list);
				for(unsigned int index=0;index<list.length();index++)
				{
					MObject BlendNode;
					list.getDependNode(index,BlendNode);
					MFnDependencyNode BlendFn(BlendNode);

					MString materialA;
					BlendFn.findPlug("MaterialA").getValue(materialA);
					cout<<"===========blend test==========="<<endl;
					cout<<materialA.asChar()<<endl;

					string mA(materialA.asChar());
					std::map<string , yafaray::material_t *>::iterator iterA=materialMap.find(mA);
					if(iterA==materialMap.end())
					{
						MGlobal::displayError("(yafaray blend material) can't find material 1, please check your input string!");
						break;
					}

					MString materialB;
					BlendFn.findPlug("MaterialB").getValue(materialB);
					string mB(materialB.asChar());
					std::map<string , yafaray::material_t *>::iterator iterB=materialMap.find(mB);
					if(iterB==materialMap.end())
					{
						MGlobal::displayError("(yafaray blend material) can't find material 2, please check your input string!");
						break;
					}

					yI.paramsClearAll();

					yI.paramsSetString("type","blend_mat");
					yI.paramsSetString("material1",materialA.asChar());				
					yI.paramsSetString("material2",materialB.asChar());

					float blendValue;
					BlendFn.findPlug("BlendValue").getValue(blendValue);
					yI.paramsSetFloat("blend_value",blendValue);

					yI.paramsEndList();

					const char* blendName=BlendFn.name().asChar();
					yafaray::material_t* blendMat=yI.createMaterial(blendName);
					string bName(blendName);
					cout<<"================blend materials=============="<<endl;
					cout<<bName<<endl;
					materialMap[bName]=blendMat;


				}
			}
	}

	//if a mesh doesn't have a yaf shader, then give it a default one

	//...got problems, don't use this now...reason is in readObject

		//yI.paramsClearAll();
		//yI.paramsSetString("type","shinydiffusemat");
		//yI.paramsSetColor("color",1.0f,0.0f,0.0f);

		//yI.paramsSetColor("mirror_color",1.0f,1.0f,1.0f);
		//yI.paramsSetFloat("diffuse_reflect",1.0f);
		//yI.paramsSetFloat("specular_reflect",0.0f);
		//yI.paramsSetFloat("transparency",0.0f);
		//yI.paramsSetFloat("translucency",0.0f);
		//yI.paramsSetFloat("transmit_filter",1.0f);
		//yI.paramsSetFloat("emit",0.0f);
		//yI.paramsSetBool("fresnel_effect",false);
		//yI.paramsSetFloat("IOR",1.0f);

		//yI.paramsEndList();

		//yafaray::material_t* shinyDiffuseMat=yI.createMaterial("yafDefaultMaterial");
		//materialMap["yafDefaultMaterial"]=shinyDiffuseMat;


	


	return stat;
}