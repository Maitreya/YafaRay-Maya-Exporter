#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bReadShader.h"

#include<maya/MString.h>
#include<maya/MGlobal.h>
#include<maya/MStringArray.h>
#include<maya/MSelectionList.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MPlug.h>
#include <maya/MPlugArray.h>
#include<maya/MFnNumericData.h>
#include<interface/yafrayinterface.h>
#include<map>
#include<string>
using namespace yafaray;
using namespace std;

MStatus getShader::readShader ( yafrayInterface_t &yI , std::map<string , yafaray::material_t*> &materialMap, std::map<string, yafaray::texture_t*> &textureMap)
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

					

					//link texture to material
					//find if there is a texture layer node connect to textureLayer attribute
					MPlug texLayerPlug=ShinyDiffuseFn.findPlug("TextureLayer");
					MPlugArray srcPlugs;
					texLayerPlug.connectedTo(srcPlugs,true,false);
					//if there is any thing connect to textureLayer attribute
					if (srcPlugs.length()>0)
					{
						MString nType;
						MString nName;
						nodeType(srcPlugs[0],nType,nName);
						cout<<nType.asChar()<<endl;
						if (nType=="yafTexLayer")
						{
							//textureMap has already got a lot of textures
							readTexLayer(yI, nName, textureMap);
						}
					}
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

MStatus getShader::readTexture(yafaray::yafrayInterface_t &yI, std::map<string,yafaray::texture_t*> &textureMap)
{
	MStatus stat;

	MString getImageCmd("ls -type yafImageTexture");
	MStringArray getImageResult;
	MGlobal::executeCommand(getImageCmd,getImageResult);
	if (getImageResult.length()>0)
	{
		for (unsigned int i=0;i<getImageResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getImageResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject imageTexNode;
				list.getDependNode(index,imageTexNode);
				MFnDependencyNode imageTexFn(imageTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","image");

				float imageFloats;
				bool imageBools;
				int imageInts;

				cout<<"================image texture=============="<<endl;

				MString fileName;
				imageTexFn.findPlug("ImageFile").getValue(fileName);
				cout<<fileName.asChar()<<endl;
				yI.paramsSetString("filename",fileName.asChar());

				yI.paramsSetFloat("gamma",1.8);

				imageTexFn.findPlug("UseAlpha").getValue(imageBools);
				yI.paramsSetBool("use_alpha",imageBools);

				imageTexFn.findPlug("CalculateAlpha").getValue(imageBools);
				yI.paramsSetBool("calc_alpha",imageBools);

				imageTexFn.findPlug("NormalMap").getValue(imageBools);
				yI.paramsSetBool("normalmap",imageBools);

				imageTexFn.findPlug("RepeatX").getValue(imageInts);
				yI.paramsSetInt("xrepeat",imageInts);

				imageTexFn.findPlug("RepeatY").getValue(imageInts);
				yI.paramsSetInt("yrepeat",imageInts);

				imageTexFn.findPlug("MinX").getValue(imageFloats);
				yI.paramsSetFloat("cropmin_x",imageFloats);

				imageTexFn.findPlug("MinY").getValue(imageFloats);
				yI.paramsSetFloat("cropmin_y",imageFloats);

				imageTexFn.findPlug("MaxX").getValue(imageFloats);
				yI.paramsSetFloat("cropmax_x",imageFloats);

				imageTexFn.findPlug("MaxY").getValue(imageFloats);
				yI.paramsSetFloat("cropmax_y",imageFloats);

				yI.paramsEndList();

				const char* imageTexName=imageTexFn.name().asChar();
				yafaray::texture_t* imageMat=yI.createTexture(imageTexName);
				string iName(imageTexName);
				cout<<iName<<endl;
				textureMap[iName]=imageMat;

			}
		}
	}

	MString getVoronoiCmd("ls -type yafVoronoiTexture");
	MStringArray getVoronoiResult;
	MGlobal::executeCommand(getVoronoiCmd,getVoronoiResult);
	if (getVoronoiResult.length()>0)
	{
		for (unsigned int i=0;i<getVoronoiResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getVoronoiResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject voronoiTexNode;
				list.getDependNode(index,voronoiTexNode);
				MFnDependencyNode voronoiTexFn(voronoiTexNode);

				float voronoiFloats;

				yI.paramsClearAll();
				yI.paramsSetString("type","voronoi");

				short voColorType;
				voronoiTexFn.findPlug("ColorType").getValue(voColorType);
				switch (voColorType)
				{
				case 0:
					yI.paramsSetString("color_type","int");
					break;
				case 1:
					yI.paramsSetString("color_type","col1");
					break;
				case 2:
					yI.paramsSetString("color_type","col2");
					break;
				case 3:
					yI.paramsSetString("color_type","col3");
					break;
				}

				voronoiTexFn.findPlug("Weight1").getValue(voronoiFloats);
				yI.paramsSetFloat("weight1",voronoiFloats);

				voronoiTexFn.findPlug("Weight2").getValue(voronoiFloats);
				yI.paramsSetFloat("weight2",voronoiFloats);

				voronoiTexFn.findPlug("Weight3").getValue(voronoiFloats);
				yI.paramsSetFloat("weight3",voronoiFloats);

				voronoiTexFn.findPlug("Weight4").getValue(voronoiFloats);
				yI.paramsSetFloat("weight4",voronoiFloats);

				voronoiTexFn.findPlug("Exponent").getValue(voronoiFloats);
				yI.paramsSetFloat("mk_exponent",voronoiFloats);

				voronoiTexFn.findPlug("Intensity").getValue(voronoiFloats);
				yI.paramsSetFloat("intensity",voronoiFloats);

				voronoiTexFn.findPlug("VoronoiSize").getValue(voronoiFloats);
				yI.paramsSetFloat("size",voronoiFloats);

				short distMetric;
				voronoiTexFn.findPlug("DistanceMetric").getValue(distMetric);
				switch (distMetric)
				{
				case 0:
					yI.paramsSetString("distance_metric","actual");
					break;
				case 1:
					yI.paramsSetString("distance_metric","squared");
					break;
				case 2:
					yI.paramsSetString("distance_metric","manhattan");
					break;
				case 3:
					yI.paramsSetString("distance_metric","chebychev");
					break;
				case 4:
					yI.paramsSetString("distance_metric","minkovsky_half");
					break;
				case 5:
					yI.paramsSetString("distance_metric","minkovsky_four");
					break;
				case 6:
					yI.paramsSetString("distance_metric","minkovsky");
					break;
				}

				yI.paramsEndList();

				const char* voronoiTexName=voronoiTexFn.name().asChar();
				yafaray::texture_t* voMat=yI.createTexture(voronoiTexName);
				string vName(voronoiTexName);
				cout<<vName<<endl;
				textureMap[vName]=voMat;
			}
		}
	}
	return stat;
}
MStatus getShader::nodeType(MPlug srcPlug, MString &nType, MString &nName)
{
	MStatus stat;
	MObject nodeObj=srcPlug.node();
	MFnDependencyNode nodeFn(nodeObj);

	nType=nodeFn.typeName();
	nName=nodeFn.name();
	cout<<"=============test nodeType============="<<endl;
	cout<<nType.asChar()<<endl;
	cout<<nName.asChar()<<endl;

	return stat;
}
MStatus getShader::readTexLayer(yafrayInterface_t &yI, MString layerName, std::map<string,yafaray::texture_t*> &textureMap)
{
	MStatus stat;
	MSelectionList list;
	MGlobal::getSelectionListByName(layerName,list);
	for (unsigned int index=0; index<list.length(); index++)
	{
		MObject layerObject;
		list.getDependNode(index,layerObject);
		MFnDependencyNode layerFn(layerObject);

		//find the texture connected to the first input
		MPlug inputP1=layerFn.findPlug("LayerInput1");
		MPlugArray srcP1;
		inputP1.connectedTo(srcP1,true,false);

		if (srcP1.length()==0)
		{
			cout<<"nothing connected to input 1"<<endl;
			break;
		}

		//find the name of the input texture
		MString nType,nName;
		nodeType(srcP1[0],nType,nName);

		//check if the input thing is a yafaray texture
		string nodeName(nName.asChar());
		std::map<string , yafaray::texture_t *>::iterator iter=textureMap.find(nodeName);
		if(iter==textureMap.end())
		{
			cout<<nodeName<<" is not a yafaray texture"<<endl;
			break;
		}

		cout<<"=================texture layer function test================="<<endl;


		yI.paramsPushList();
		yI.paramsSetString("element","shader_node");
		yI.paramsSetString("type","layer");
		yI.paramsSetString("name",layerName.asChar());
		MString mapperName=nName+"Mapper";
		yI.paramsSetString("input",mapperName.asChar());

		short mixMode;
		layerFn.findPlug("MixMethod1").getValue(mixMode);
		yI.paramsSetInt("mode",mixMode);

		//haven't added them
		yI.paramsSetBool("stencil",false);
		yI.paramsSetBool("negative",false);
		yI.paramsSetBool("noRGB",false);
		yI.paramsSetColor("def_col",1.0,0.5,0.5);
		yI.paramsSetFloat("def_val",1.0);
		yI.paramsSetFloat("colfac",1.0);
		yI.paramsSetFloat("valfac",1.0);
		yI.paramsSetBool("color_input",true);
		yI.paramsSetBool("use_alpha",false);
		
		yI.paramsSetColor("upper_color",0,0,0);
		yI.paramsSetFloat("upper_value",1.0);

		yI.paramsSetBool("do_color",false);
		yI.paramsSetBool("do_scalar",true);

		readTexMapping(yI,nName);

	}

	return stat;
}

MStatus getShader::readTexMapping(yafrayInterface_t &yI, MString textureName)
{
	MStatus stat;
	MSelectionList list;
	MGlobal::getSelectionListByName(textureName,list);
	for (unsigned int index=0; index<list.length(); index++)
	{
		MObject textureNode;
		list.getDependNode(index,textureNode);
		MFnDependencyNode texFn(textureNode);

		yI.paramsPushList();
		yI.paramsSetString("element","shader_node");
		yI.paramsSetString("type","texture_mapper");
		//mapper name can't be the same with texture name
		MString mapperName=textureName+"Mapper";
		yI.paramsSetString("name",mapperName.asChar());
		yI.paramsSetString("texture",textureName.asChar());

		short texcoMethod;
		texFn.findPlug("MappingMethod").getValue( texcoMethod);
		switch (texcoMethod)
		{
		case 0:
			yI.paramsSetString("texco","uv");
			break;
		case 1:
			yI.paramsSetString("texco","orco");
			break;
		case 2:
			yI.paramsSetString("texco","global");
			break;
		case 3:
			yI.paramsSetString("texco","window");
			break;
		}

		short mapMethod;
		texFn.findPlug("TextureCoordinate").getValue(mapMethod);
		switch (mapMethod)
		{
		case 0:
			yI.paramsSetString("mapping","plain");
			break;
		case 1:
			yI.paramsSetString("mapping","cube");
			break;
		case 2:
			yI.paramsSetString("mapping","tube");
			break;
		case 3:
			yI.paramsSetString("mapping","sphere");
			break;
		}
	}

	return stat;
}