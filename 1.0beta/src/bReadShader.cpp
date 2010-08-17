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

MStatus getShader::readTexture(yafaray::yafrayInterface_t &yI, std::map<string,yafaray::texture_t*> &textureMap)
{
	MStatus stat;

	MString getImageCmd="ls -type yafImageTexture";
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

				yI.paramsSetString("clipping","repeat");

				yI.paramsEndList();

				const char* imageTexName=imageTexFn.name().asChar();
				yafaray::texture_t* imageMat=yI.createTexture(imageTexName);
				string iName(imageTexName);
				cout<<iName<<endl;
				textureMap[iName]=imageMat;

			}
		}
	}

	MString getVoronoiCmd="ls -type yafVoronoiTexture";
	MStringArray getVoronoiResut;
	MGlobal::executeCommand(getVoronoiCmd,getVoronoiResut);
	if (getVoronoiResut.length()>0)
	{
		for (unsigned int i=0;i<getVoronoiResut.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getVoronoiResut[i],list);
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

	MString getBlendCmd="ls -type yafBlendTexture";
	MStringArray getBlendResult;
	MGlobal::executeCommand(getBlendCmd,getBlendResult);
	if (getBlendResult.length()>0)
	{
		for (unsigned int i=0;i<getBlendResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getBlendResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject blendTexNode;
				list.getDependNode(index,blendTexNode);
				MFnDependencyNode blendTexFn(blendTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","blend");
				short stype;
				blendTexFn.findPlug("Stype").getValue(stype);
				switch (stype)
				{
				case 0:
					yI.paramsSetString("stype","lin");
					break;
				case 1:
					yI.paramsSetString("stype","quad");
					break;
				case 2:
					yI.paramsSetString("stype","ease");
					break;
				case 3:
					yI.paramsSetString("stype","diag");
					break;
				case 4:
					yI.paramsSetString("stype","sphere");
					break;
				case 5:
					yI.paramsSetString("stype","halo");
					break;
				}

				yI.paramsEndList();

				const char* blendTexName=blendTexFn.name().asChar();
				yafaray::texture_t* blendMat=yI.createTexture(blendTexName);
				string bName(blendTexName);
				cout<<bName<<endl;
				textureMap[bName]=blendMat;
			}
		}
	}

	MString getCloudCmd="ls -type yafCloudTexture";
	MStringArray cloudResult;
	MGlobal::executeCommand(getCloudCmd,cloudResult);
	if (cloudResult.length()>0)
	{
		for (unsigned int i=0;i<cloudResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(cloudResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject cloudTexNode;
				list.getDependNode(index,cloudTexNode);
				MFnDependencyNode cloudTexFn(cloudTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","clouds");

				float size;
				cloudTexFn.findPlug("CloudSize").getValue(size);
				yI.paramsSetFloat("size",size);

				bool hard;
				cloudTexFn.findPlug("Hard").getValue(hard);
				yI.paramsSetBool("hard",hard);

				int depth;
				cloudTexFn.findPlug("CloudDepth").getValue(depth);
				yI.paramsSetInt("depth",depth);

				short noiseType;
				cloudTexFn.findPlug("NoiseType").getValue(noiseType);
				switch (noiseType)
				{
				case 0:
					yI.paramsSetString("noise_type","newperlin");
					break;
				case 1:
					yI.paramsSetString("noise_type","stdperlin");
					break;
				case 2:
					yI.paramsSetString("noise_type","voronoi_f1");
					break;
				case 3:
					yI.paramsSetString("noise_type","voronoi_f2");
					break;
				case 4:
					yI.paramsSetString("noise_type","voronoi_f3");
					break;
				case 5:
					yI.paramsSetString("noise_type","voronoi_f4");
					break;
				case 6:
					yI.paramsSetString("noise_type","voronoi_f2f1");
					break;
				case 7:
					yI.paramsSetString("noise_type","voronoi_crackle");
					break;
				case 8:
					yI.paramsSetString("noise_type","cellnoise");
					break;
				}

				yI.paramsEndList();

				const char* cloudTexName=cloudTexFn.name().asChar();
				yafaray::texture_t* cloudMat=yI.createTexture(cloudTexName);
				string cName(cloudTexName);
				cout<<cName<<endl;
				textureMap[cName]=cloudMat;
			}
		}
	}

	MString getWoodCmd="ls -type yafWoodTexture";
	MStringArray getWoodResult;
	MGlobal::executeCommand(getWoodCmd,getWoodResult);
	if (getWoodResult.length()>0)
	{
		for (unsigned int i=0;i<getWoodResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getWoodResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject woodTexNode;
				list.getDependNode(index,woodTexNode);
				MFnDependencyNode woodTexFn(woodTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","wood");

				yI.paramsSetInt("depth",0);
				
				float turb;
				woodTexFn.findPlug("Turbulence").getValue(turb);
				yI.paramsSetFloat("turbulence",turb);

				float size;
				woodTexFn.findPlug("WoodSize").getValue(size);
				yI.paramsSetFloat("size",size);

				bool hard;
				woodTexFn.findPlug("Hard").getValue(hard);
				yI.paramsSetBool("hard",hard);

				short woodType;
				woodTexFn.findPlug("WoodType").getValue(woodType);
				switch (woodType)
				{
				case 0:
					yI.paramsSetString("wood_type","bands");
					break;
				case 1:
					yI.paramsSetString("wood_type","rings");
					break;
				}

				short noiseType;
				woodTexFn.findPlug("NoiseType").getValue(noiseType);
				switch (noiseType)
				{
				case 0:
					yI.paramsSetString("noise_type","newperlin");
					break;
				case 1:
					yI.paramsSetString("noise_type","stdperlin");
					break;
				case 2:
					yI.paramsSetString("noise_type","voronoi_f1");
					break;
				case 3:
					yI.paramsSetString("noise_type","voronoi_f2");
					break;
				case 4:
					yI.paramsSetString("noise_type","voronoi_f3");
					break;
				case 5:
					yI.paramsSetString("noise_type","voronoi_f4");
					break;
				case 6:
					yI.paramsSetString("noise_type","voronoi_f2f1");
					break;
				case 7:
					yI.paramsSetString("noise_type","voronoi_crackle");
					break;
				case 8:
					yI.paramsSetString("noise_type","cellnoise");
					break;
				}
				if (noiseType==1)
				{
					yI.paramsSetString("shape","tri");
				}
				else
				{
					yI.paramsSetString("shape","sin");
				}

				yI.paramsEndList();

				const char* woodTexName=woodTexFn.name().asChar();
				yafaray::texture_t* woodMat=yI.createTexture(woodTexName);
				string wName(woodTexName);
				cout<<wName<<endl;
				textureMap[wName]=woodMat;
			}
		}
	}

	MString getMarbleCmd="ls -type yafMarbleTexture";
	MStringArray marbleResult;
	MGlobal::executeCommand(getMarbleCmd,marbleResult);
	if (marbleResult.length()>0)
	{
		for (unsigned int i=0;i<marbleResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(marbleResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject marbleTexNode;
				list.getDependNode(index,marbleTexNode);
				MFnDependencyNode marbleTexFn(marbleTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","marble");

				int depth;
				marbleTexFn.findPlug("Depth").getValue(depth);
				yI.paramsSetInt("depth",depth);

				float turb;
				marbleTexFn.findPlug("Turbulence").getValue(turb);
				yI.paramsSetFloat("turbulence",turb);

				float size;
				marbleTexFn.findPlug("MarbleSize").getValue(size);
				yI.paramsSetFloat("size",size);

				bool hard;
				marbleTexFn.findPlug("Hard").getValue(hard);
				yI.paramsSetBool("hard",hard);

				//float sharpness;
				//can't understand this

				short noiseType;
				marbleTexFn.findPlug("NoiseType").getValue(noiseType);
				switch (noiseType)
				{
				case 0:
					yI.paramsSetString("noise_type","newperlin");
					break;
				case 1:
					yI.paramsSetString("noise_type","stdperlin");
					break;
				case 2:
					yI.paramsSetString("noise_type","voronoi_f1");
					break;
				case 3:
					yI.paramsSetString("noise_type","voronoi_f2");
					break;
				case 4:
					yI.paramsSetString("noise_type","voronoi_f3");
					break;
				case 5:
					yI.paramsSetString("noise_type","voronoi_f4");
					break;
				case 6:
					yI.paramsSetString("noise_type","voronoi_f2f1");
					break;
				case 7:
					yI.paramsSetString("noise_type","voronoi_crackle");
					break;
				case 8:
					yI.paramsSetString("noise_type","cellnoise");
					break;
				}

				if (noiseType==1)
				{
					yI.paramsSetString("shape","tri");
				}
				else
				{
					yI.paramsSetString("shape","sin");
				}

				yI.paramsEndList();

				const char* marbleTexName=marbleTexFn.name().asChar();
				yafaray::texture_t* marbleMat=yI.createTexture(marbleTexName);
				string mName(marbleTexName);
				cout<<mName<<endl;
				textureMap[mName]=marbleMat;
			}
		}
	}

	MString getMusgraveCmd="ls -type yafMusgraveTexture";
	MStringArray musgraveResult;
	MGlobal::executeCommand(getMusgraveCmd,musgraveResult);
	if (musgraveResult.length()>0)
	{
		for (unsigned int i=0;i<musgraveResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(musgraveResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject musgraveTexNode;
				list.getDependNode(index,musgraveTexNode);
				MFnDependencyNode musgraveTexFn(musgraveTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","musgrave");

				short musType;
				musgraveTexFn.findPlug("MusgraveType").getValue(musType);
				switch (musType)
				{
				case 0:
					yI.paramsSetString("musgrave_type","fBm");
					break;
				case 1:
					yI.paramsSetString("musgrave_type","multifractal");
					break;
				case 2:
					yI.paramsSetString("musgrave_type","ridgedmf");
					break;
				case 3:
					yI.paramsSetString("musgrave_type","hybridmf");
					break;
				case 4:
					yI.paramsSetString("musgrave_type","heteroterrain");
					break;
				}

				short noiseType;
				musgraveTexFn.findPlug("NoiseType").getValue(noiseType);
				switch (noiseType)
				{
				case 0:
					yI.paramsSetString("noise_type","newperlin");
					break;
				case 1:
					yI.paramsSetString("noise_type","stdperlin");
					break;
				case 2:
					yI.paramsSetString("noise_type","voronoi_f1");
					break;
				case 3:
					yI.paramsSetString("noise_type","voronoi_f2");
					break;
				case 4:
					yI.paramsSetString("noise_type","voronoi_f3");
					break;
				case 5:
					yI.paramsSetString("noise_type","voronoi_f4");
					break;
				case 6:
					yI.paramsSetString("noise_type","voronoi_f2f1");
					break;
				case 7:
					yI.paramsSetString("noise_type","voronoi_crackle");
					break;
				case 8:
					yI.paramsSetString("noise_type","cellnoise");
					break;
				}

				float musFloats;
				musgraveTexFn.findPlug("H").getValue(musFloats);
				yI.paramsSetFloat("H",musFloats);

				musgraveTexFn.findPlug("Lacunarity").getValue(musFloats);
				yI.paramsSetFloat("lacunarity",musFloats);

				musgraveTexFn.findPlug("Octaves").getValue(musFloats);
				yI.paramsSetFloat("octaves",musFloats);

				musgraveTexFn.findPlug("MusgraveSize").getValue(musFloats);
				yI.paramsSetFloat("size",musFloats);

				musgraveTexFn.findPlug("Intensity").getValue(musFloats);
				yI.paramsSetFloat("intensity",musFloats);

				yI.paramsEndList();

				const char* musTexName=musgraveTexFn.name().asChar();
				yafaray::texture_t* musfMat=yI.createTexture(musTexName);
				string mName(musTexName);
				cout<<mName<<endl;
				textureMap[mName]=musfMat;
			}
		}
	}

	MString getDisnoiseCmd="ls -type yafDisnoiseTexture";
	MStringArray getDisnoiseResult;
	MGlobal::executeCommand(getDisnoiseCmd,getDisnoiseResult);
	if (getDisnoiseResult.length()>0)
	{
		for (unsigned int i=0;i<getDisnoiseResult.length();i++)
		{
			MSelectionList list;
			MGlobal::getSelectionListByName(getDisnoiseResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MObject disnoiseTexNode;
				list.getDependNode(index,disnoiseTexNode);
				MFnDependencyNode disnoiseTexFn(disnoiseTexNode);

				yI.paramsClearAll();
				yI.paramsSetString("type","distorted_noise");

				float distort;
				disnoiseTexFn.findPlug("Distort").getValue(distort);
				yI.paramsSetFloat("distort",distort);

				float size;
				disnoiseTexFn.findPlug("DisnoiseSize").getValue(size);
				yI.paramsSetFloat("size",size);

				short noiseType1;
				disnoiseTexFn.findPlug("NoiseType1").getValue(noiseType1);
				switch (noiseType1)
				{
				case 0:
					yI.paramsSetString("noise_type1","newperlin");
					break;
				case 1:
					yI.paramsSetString("noise_type1","stdperlin");
					break;
				case 2:
					yI.paramsSetString("noise_type1","voronoi_f1");
					break;
				case 3:
					yI.paramsSetString("noise_type1","voronoi_f2");
					break;
				case 4:
					yI.paramsSetString("noise_type1","voronoi_f3");
					break;
				case 5:
					yI.paramsSetString("noise_type1","voronoi_f4");
					break;
				case 6:
					yI.paramsSetString("noise_type1","voronoi_f2f1");
					break;
				case 7:
					yI.paramsSetString("noise_type1","voronoi_crackle");
					break;
				case 8:
					yI.paramsSetString("noise_type1","cellnoise");
					break;
				}
				short noiseType2;
				disnoiseTexFn.findPlug("NoiseType2").getValue(noiseType2);
				switch (noiseType2)
				{
				case 0:
					yI.paramsSetString("noise_type2","newperlin");
					break;
				case 1:
					yI.paramsSetString("noise_type2","stdperlin");
					break;
				case 2:
					yI.paramsSetString("noise_type2","voronoi_f1");
					break;
				case 3:
					yI.paramsSetString("noise_type2","voronoi_f2");
					break;
				case 4:
					yI.paramsSetString("noise_type2","voronoi_f3");
					break;
				case 5:
					yI.paramsSetString("noise_type2","voronoi_f4");
					break;
				case 6:
					yI.paramsSetString("noise_type2","voronoi_f2f1");
					break;
				case 7:
					yI.paramsSetString("noise_type2","voronoi_crackle");
					break;
				case 8:
					yI.paramsSetString("noise_type2","cellnoise");
					break;
				}

				yI.paramsEndList();

				const char* disnoiseTexName=disnoiseTexFn.name().asChar();
				yafaray::texture_t* disMat=yI.createTexture(disnoiseTexName);
				string dName(disnoiseTexName);
				cout<<dName<<endl;
				textureMap[dName]=disMat;
			}
		}
	}

	return stat;
}
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

					//****************************texture node**************************//
					MString nType;
					MString nName;

					MPlug mirrorTexPlug=GlassFn.findPlug("MirrorTextureLayer");
					MPlugArray srcMirPlugs;
					mirrorTexPlug.connectedTo(srcMirPlugs,true,false);
					MString shaderMirLayer;
					if (srcMirPlugs.length()>0)
					{
						nodeType(srcMirPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							readTexLayer(yI,nName, textureMap, mirrorColor, shaderMirLayer);
						}
					}

					MPlug filterTexPlug=GlassFn.findPlug("FilterTextureLayer");
					MPlugArray srcFilPlugs;
					filterTexPlug.connectedTo(srcFilPlugs,true,false);
					MString shaderFilLayer;
					if (srcFilPlugs.length()>0)
					{
						nodeType(srcFilPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							readTexLayer(yI,nName, textureMap, filterColor, shaderFilLayer);
						}
					}

					MPlug bumpLayerPlug=GlassFn.findPlug("BumpTextureLayer");
					MPlugArray srcBumpPlugs;
					bumpLayerPlug.connectedTo(srcBumpPlugs,true,false);
					MString shaderBumpLayer;
					if (srcBumpPlugs.length()>0)
					{
						nodeType(srcBumpPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							//get into this texlayer node
							readTexLayer(yI,nName, textureMap, mirrorColor, shaderBumpLayer);
						}
					}

					yI.paramsEndList();
					if (shaderMirLayer!="")
					{
						yI.paramsSetString("mirror_color_shader",shaderMirLayer.asChar());
					}
					if (shaderFilLayer!="")
					{
						yI.paramsSetString("filter_color_shader",shaderFilLayer.asChar());
					}
					if (shaderBumpLayer!="")
					{
						yI.paramsSetString("bump_shader",shaderBumpLayer.asChar());
					}				

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

				float colorsR,colorsG,colorsB;
				float roughGlassFloats;

				roughGlassFn.findPlug("Exponent").getValue(roughGlassFloats);
				yI.paramsSetFloat("exponent",roughGlassFloats);

				roughGlassFn.findPlug("Roughtness").getValue(roughGlassFloats);
				yI.paramsSetFloat("alpha",roughGlassFloats);

				MObject absorbColor;
				roughGlassFn.findPlug("AbsorbColor").getValue(absorbColor);
				MFnNumericData absorbData(absorbColor);
				absorbData.getData(colorsR,colorsG,colorsB);
				yI.paramsSetColor("absorption",colorsR,colorsG,colorsB);

				roughGlassFn.findPlug("AbsorbDistance").getValue(roughGlassFloats);
				yI.paramsSetFloat("absorption_dist",roughGlassFloats);

				MObject filterColor;
				roughGlassFn.findPlug("FilterColor").getValue(filterColor);
				MFnNumericData filterData(filterColor);
				filterData.getData(colorsR,colorsG,colorsB);
				yI.paramsSetColor("filter_color",colorsR,colorsG,colorsB);

				MObject mirrorColor;
				roughGlassFn.findPlug("MirrorColor").getValue(mirrorColor);
				MFnNumericData mirrorData(mirrorColor);
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

				//****************************texture node**************************//
				MString nType;
				MString nName;

				MPlug mirrorTexPlug=roughGlassFn.findPlug("MirrorTextureLayer");
				MPlugArray srcMirPlugs;
				mirrorTexPlug.connectedTo(srcMirPlugs,true,false);
				MString shaderMirLayer;
				if (srcMirPlugs.length()>0)
				{
					nodeType(srcMirPlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						readTexLayer(yI,nName, textureMap, mirrorColor, shaderMirLayer);
					}
				}

				MPlug filterTexPlug=roughGlassFn.findPlug("FilterTextureLayer");
				MPlugArray srcFilPlugs;
				filterTexPlug.connectedTo(srcFilPlugs,true,false);
				MString shaderFilLayer;
				if (srcFilPlugs.length()>0)
				{
					nodeType(srcFilPlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						readTexLayer(yI,nName, textureMap, filterColor, shaderFilLayer);
					}
				}

				MPlug bumpLayerPlug=roughGlassFn.findPlug("BumpTextureLayer");
				MPlugArray srcBumpPlugs;
				bumpLayerPlug.connectedTo(srcBumpPlugs,true,false);
				MString shaderBumpLayer;
				if (srcBumpPlugs.length()>0)
				{
					nodeType(srcBumpPlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						//get into this texlayer node
						readTexLayer(yI,nName, textureMap, mirrorColor, shaderBumpLayer);
					}
				}

				yI.paramsEndList();
				if (shaderMirLayer!="")
				{
					yI.paramsSetString("mirror_color_shader",shaderMirLayer.asChar());
				}
				if (shaderFilLayer!="")
				{
					yI.paramsSetString("filter_color_shader",shaderFilLayer.asChar());
				}
				if (shaderBumpLayer!="")
				{
					yI.paramsSetString("bump_shader",shaderBumpLayer.asChar());
				}				

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

					float coatedR,coatedG,coatedB;
					float coatedFloats;

					yI.paramsClearAll();
					yI.paramsSetString("type","coated_glossy");

					MObject diffuseColor;
					CoatedGlossyFn.findPlug("DiffuseColor").getValue(diffuseColor);
					MFnNumericData diffuseData(diffuseColor);
					diffuseData.getData(coatedR,coatedG,coatedB);
					yI.paramsSetColor("diffuse_color",coatedR,coatedG,coatedB);

					CoatedGlossyFn.findPlug("DiffuseReflection").getValue(coatedFloats);
					yI.paramsSetFloat("diffuse_reflect",coatedFloats);

					MObject glossyColor;
					CoatedGlossyFn.findPlug("GlossyColor").getValue(glossyColor);
					MFnNumericData glossyData(glossyColor);
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

					//****************************texture node**************************//
					MString nType;
					MString nName;

					MPlug diffuseTexPlug=CoatedGlossyFn.findPlug("DiffuseTextureLayer");
					MPlugArray srcDiffPlugs;
					diffuseTexPlug.connectedTo(srcDiffPlugs,true,false);
					MString shaderDiffLayer;
					if (srcDiffPlugs.length()>0)
					{
						nodeType(srcDiffPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							readTexLayer(yI,nName, textureMap, diffuseColor, shaderDiffLayer);
						}
					}

					MPlug glossyTexPlug=CoatedGlossyFn.findPlug("GlossyTextureLayer");
					MPlugArray srcGlossyPlugs;
					glossyTexPlug.connectedTo(srcGlossyPlugs,true,false);
					MString shaderGlossyLayer;
					if (srcGlossyPlugs.length()>0)
					{
						nodeType(srcGlossyPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							readTexLayer(yI,nName, textureMap, glossyColor, shaderGlossyLayer);
						}
					}

					MPlug glossyReTexPlug=CoatedGlossyFn.findPlug("GlossyReflectionTexLayer");
					MPlugArray srcGlossyRePlugs;
					glossyReTexPlug.connectedTo(srcGlossyRePlugs,true,false);
					MString shaderGlossyReLayer;
					if (srcGlossyRePlugs.length()>0)
					{
						nodeType(srcGlossyRePlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							readTexLayer(yI,nName, textureMap, glossyColor, shaderGlossyReLayer);
						}
					}

					MPlug bumpLayerPlug=CoatedGlossyFn.findPlug("BumpTextureLayer");
					MPlugArray srcBumpPlugs;
					bumpLayerPlug.connectedTo(srcBumpPlugs,true,false);
					MString shaderBumpLayer;
					if (srcBumpPlugs.length()>0)
					{
						nodeType(srcBumpPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							readTexLayer(yI,nName, textureMap, diffuseColor, shaderBumpLayer);
						}
					}

					yI.paramsEndList();
					if (shaderDiffLayer!="")
					{
						yI.paramsSetString("diffuse_shader",shaderDiffLayer.asChar());
					}
					if (shaderGlossyLayer!="")
					{
						yI.paramsSetString("glossy_shader",shaderGlossyLayer.asChar());
					}
					if (shaderGlossyReLayer!="")
					{
						yI.paramsSetString("glossy_reflect_shader",shaderGlossyReLayer.asChar());
					}
					if (shaderBumpLayer!="")
					{
						yI.paramsSetString("bump_shader",shaderBumpLayer.asChar());
					}				

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

				float glossyR,glossyG,glossyB;
				float glossyFloats;

				yI.paramsClearAll();
				yI.paramsSetString("type","coated_glossy");

				MObject diffuseColor;
				GlossyFn.findPlug("DiffuseColor").getValue(diffuseColor);
				MFnNumericData diffuseData(diffuseColor);
				diffuseData.getData(glossyR,glossyG,glossyB);
				yI.paramsSetColor("diffuse_color",glossyR,glossyG,glossyB);

				GlossyFn.findPlug("DiffuseReflection").getValue(glossyFloats);
				yI.paramsSetFloat("diffuse_reflect",glossyFloats);

				MObject glossyColor;
				GlossyFn.findPlug("GlossyColor").getValue(glossyColor);
				MFnNumericData glossyData(glossyColor);
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

				//****************************texture node**************************//
				MString nType;
				MString nName;

				MPlug diffuseTexPlug=GlossyFn.findPlug("DiffuseTextureLayer");
				MPlugArray srcDiffPlugs;
				diffuseTexPlug.connectedTo(srcDiffPlugs,true,false);
				MString shaderDiffLayer;
				if (srcDiffPlugs.length()>0)
				{
					nodeType(srcDiffPlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						readTexLayer(yI,nName, textureMap, diffuseColor, shaderDiffLayer);
					}
				}

				MPlug glossyTexPlug=GlossyFn.findPlug("GlossyTextureLayer");
				MPlugArray srcGlossyPlugs;
				glossyTexPlug.connectedTo(srcGlossyPlugs,true,false);
				MString shaderGlossyLayer;
				if (srcGlossyPlugs.length()>0)
				{
					nodeType(srcGlossyPlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						readTexLayer(yI,nName, textureMap, glossyColor, shaderGlossyLayer);
					}
				}

				MPlug glossyReTexPlug=GlossyFn.findPlug("GlossyReflectionTexLayer");
				MPlugArray srcGlossyRePlugs;
				glossyReTexPlug.connectedTo(srcGlossyRePlugs,true,false);
				MString shaderGlossyReLayer;
				if (srcGlossyRePlugs.length()>0)
				{
					nodeType(srcGlossyRePlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						//the color input "diffuseColor" is useless here
						readTexLayer(yI,nName, textureMap, diffuseColor, shaderGlossyReLayer);
					}
				}

				MPlug bumpLayerPlug=GlossyFn.findPlug("BumpTextureLayer");
				MPlugArray srcBumpPlugs;
				bumpLayerPlug.connectedTo(srcBumpPlugs,true,false);
				MString shaderBumpLayer;
				if (srcBumpPlugs.length()>0)
				{
					nodeType(srcBumpPlugs[0],nType,nName);
					if (nType=="yafTexLayer")
					{
						//the color input "diffuseColor" is useless here
						readTexLayer(yI,nName, textureMap, diffuseColor, shaderBumpLayer);
					}
				}

				yI.paramsEndList();
				if (shaderDiffLayer!="")
				{
					yI.paramsSetString("diffuse_shader",shaderDiffLayer.asChar());
				}
				if (shaderGlossyLayer!="")
				{
					yI.paramsSetString("glossy_shader",shaderGlossyLayer.asChar());
				}
				if (shaderGlossyReLayer!="")
				{
					yI.paramsSetString("glossy_reflect_shader",shaderGlossyReLayer.asChar());
				}
				if (shaderBumpLayer!="")
				{
					yI.paramsSetString("bump_shader",shaderBumpLayer.asChar());
				}		

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

					//************************************texture node*****************************************//
					MString nType;
					MString nName;

					MPlug colorLayerPlug=ShinyDiffuseFn.findPlug("ColorTextureLayer");
					MPlugArray srcColorPlugs;
					colorLayerPlug.connectedTo(srcColorPlugs,true,false);
					MString shaderColorLayer;
					//if there is any thing connect to textureLayer attribute
					if (srcColorPlugs.length()>0)
					{
						nodeType(srcColorPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							//get into this texlayer node
							readTexLayer(yI,nName, textureMap, Color, shaderColorLayer);
						}
					}

					MPlug mirrorLayerPlug=ShinyDiffuseFn.findPlug("MirrorTextureLayer");
					MPlugArray srcMirrorPlugs;
					mirrorLayerPlug.connectedTo(srcMirrorPlugs,true,false);
					MString shaderMirrorLayer;
					if (srcMirrorPlugs.length()>0)
					{
						nodeType(srcMirrorPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							//get into this texlayer node
							readTexLayer(yI,nName, textureMap, Color, shaderMirrorLayer);
						}
					}

					MPlug bumpLayerPlug=ShinyDiffuseFn.findPlug("BumpTextureLayer");
					MPlugArray srcBumpPlugs;
					bumpLayerPlug.connectedTo(srcBumpPlugs,true,false);
					MString shaderBumpLayer;
					if (srcBumpPlugs.length()>0)
					{
						nodeType(srcBumpPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{
							//get into this texlayer node
							readTexLayer(yI,nName, textureMap, Color, shaderBumpLayer);
						}
					}

					yI.paramsEndList();

					if (shaderColorLayer!="")
					{
						yI.paramsSetString("diffuse_shader",shaderColorLayer.asChar());
					}
					if (shaderMirrorLayer!="")
					{
						yI.paramsSetString("mirror_color_shader",shaderMirrorLayer.asChar());
					}
					if (shaderBumpLayer!="")
					{
						yI.paramsSetString("bump_shader",shaderBumpLayer.asChar());
					}

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

					//************************************texture node*****************************************//
					MString nType;
					MString nName;
					//this color is use less here, just to be a must-be parameter...
					MObject Color;

					MPlug diffuseLayerPlug=BlendFn.findPlug("DiffuseTextureLayer");
					MPlugArray srcDiffPlugs;
					diffuseLayerPlug.connectedTo(srcDiffPlugs,true,false);
					MString shaderDiffLayer;
					//if there is any thing connect to textureLayer attribute
					if (srcDiffPlugs.length()>0)
					{
						nodeType(srcDiffPlugs[0],nType,nName);
						if (nType=="yafTexLayer")
						{

							readTexLayer(yI,nName, textureMap, Color, shaderDiffLayer);
						}
					}

					yI.paramsEndList();
					yI.paramsSetString("mask",shaderDiffLayer.asChar());

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


MStatus getShader::nodeType(MPlug srcPlug, MString &nType, MString &nName)
{
	MStatus stat;
	MObject nodeObj=srcPlug.node();
	MFnDependencyNode nodeFn(nodeObj);

	nType=nodeFn.typeName();
	nName=nodeFn.name();
	//cout<<"=============test nodeType============="<<endl;
	//cout<<nType.asChar()<<endl;
	//cout<<nName.asChar()<<endl;

	return stat;
}
MStatus getShader::readTexLayer(yafaray::yafrayInterface_t &yI, MString textureLayerName, std::map<string,yafaray::texture_t*> &textureMap, MObject color, MString &layerShader)
{
	MStatus stat;

	//this is the upper layer of the next layer
	MString upperLayer;

	//get into the texlayer
	MSelectionList list;
	MGlobal::getSelectionListByName(textureLayerName,list);
	for (unsigned int index=0; index<list.length(); index++)
	{
		MObject layerObject;
		list.getDependNode(index,layerObject);
		MFnDependencyNode layerFn(layerObject);

		//************************input 1*************************//
		MPlug inputP1=layerFn.findPlug("LayerInput1");
		MPlugArray srcP1;
		inputP1.connectedTo(srcP1,true,false);
		if (srcP1.length()>0)
		{
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
			//now really a yafaray texture connected to a material, go into it
			//input1 will be the most top layer, then it will pass it's layer name to upperLayer, which will be the "upper_layer" of the next layer
			readLayerMap(yI,nName,nType,color);
			layerShader=nName+"Layer";
			upperLayer=layerShader;
		}

		//************************input 2*************************//
		MPlug inputP2=layerFn.findPlug("LayerInput2");
		MPlugArray srcP2;
		inputP2.connectedTo(srcP2,true,false);
		if (srcP2.length()>0)
		{
			MString nType,nName;
			nodeType(srcP2[0],nType,nName);
			string nodeName(nName.asChar());
			std::map<string , yafaray::texture_t *>::iterator iter=textureMap.find(nodeName);
			if(iter==textureMap.end())
			{
				cout<<nodeName<<" is not a yafaray texture"<<endl;
				break;
			}
			readLayerMap(yI,nName,nType,color,upperLayer);
			upperLayer=nName+"Layer";
			//if there is no upper layer, this layer will be the layer on top
			if (layerShader=="")
			{
				layerShader=nName+"layer";
			}
		}

		//************************input 3*************************//
		MPlug inputP3=layerFn.findPlug("LayerInput3");
		MPlugArray srcP3;
		inputP3.connectedTo(srcP3,true,false);
		if (srcP3.length()>0)
		{
			MString nType,nName;
			nodeType(srcP3[0],nType,nName);
			string nodeName(nName.asChar());
			std::map<string , yafaray::texture_t *>::iterator iter=textureMap.find(nodeName);
			if(iter==textureMap.end())
			{
				cout<<nodeName<<" is not a yafaray texture"<<endl;
				break;
			}
			readLayerMap(yI,nName,nType,color,upperLayer);
			upperLayer=nName+"Layer";
			//if there is no upper layer, this layer will be the layer on top
			if (layerShader=="")
			{
				layerShader=nName+"layer";
			}
		}

		//************************input 4*************************//
		MPlug inputP4=layerFn.findPlug("LayerInput4");
		MPlugArray srcP4;
		inputP4.connectedTo(srcP4,true,false);
		if (srcP4.length()>0)
		{
			MString nType,nName;
			nodeType(srcP4[0],nType,nName);
			string nodeName(nName.asChar());
			std::map<string , yafaray::texture_t *>::iterator iter=textureMap.find(nodeName);
			if(iter==textureMap.end())
			{
				cout<<nodeName<<" is not a yafaray texture"<<endl;
				break;
			}
			readLayerMap(yI,nName,nType,color,upperLayer);
			upperLayer=nName+"Layer";
			//if there is no upper layer, this layer will be the layer on top
			if (layerShader=="")
			{
				layerShader=nName+"layer";
			}
		}

	}

	return stat;
}
MStatus getShader::readLayerMap(yafaray::yafrayInterface_t &yI, MString texName, MString texType, MObject color, MString upperLayer)
{
	MStatus stat;

	//set both layer and mapper here
	MSelectionList list;
	MGlobal::getSelectionListByName(texName,list);
	for (unsigned int index=0; index<list.length(); index++)
	{
		MObject texNode;
		list.getDependNode(index,texNode);
		MFnDependencyNode texFn(texNode);

		float texFloats;
		bool texBools;

		cout<<"================layer and mapper================"<<endl;

		//*******************************set layer**********************************//
		yI.paramsPushList();
		yI.paramsSetString("element","shader_node");
		yI.paramsSetString("type","layer");

		MString layerName=texName+"Layer";
		cout<<"layer name is "<<layerName.asChar()<<endl;
		MString mapperName=texName+"Mapper";
		cout<<"mapper name is "<<mapperName.asChar()<<endl;
		yI.paramsSetString("name",layerName.asChar());
		yI.paramsSetString("input",mapperName.asChar());

		if (upperLayer!="")
		{
			yI.paramsSetString("upper_layer",upperLayer.asChar());
			cout<<"upper layer of this layer is "<<upperLayer.asChar()<<endl;
		}

		MObject texColor;
		texFn.findPlug("TextureColor").getValue(texColor);
		MFnNumericData colorData(texColor);
		float cR,cG,cB;
		colorData.getData(cR,cG,cB);
		yI.paramsSetColor("def_col",cR,cG,cB);

		texFn.findPlug("TextureColorWeight").getValue(texFloats);
		yI.paramsSetFloat("colfac",texFloats);

		texFn.findPlug("DefValue").getValue(texFloats);
		yI.paramsSetFloat("def_val",texFloats);

		texFn.findPlug("ValueWeight").getValue(texFloats);
		yI.paramsSetFloat("valfac",texFloats);

		short mapperMix;
		texFn.findPlug("MixMethod").getValue(mapperMix);
		yI.paramsSetInt("mode",mapperMix);

		texFn.findPlug("Negative").getValue(texBools);
		yI.paramsSetBool("negative",texBools);

		texFn.findPlug("NoRGB").getValue(texBools);
		yI.paramsSetBool("noRGB",texBools);

		texFn.findPlug("Stencil").getValue(texBools);
		yI.paramsSetBool("stencil",texBools);

		texFn.findPlug("DoColor").getValue(texBools);
		yI.paramsSetBool("do_color",texBools);
		yI.paramsSetBool("do_scalar",!texBools);

		MFnNumericData materialColorData(color);
		float mR,mG,mB;
		materialColorData.getData(mR,mG,mB);
		//docolor or not
		if (texBools)
		{
			yI.paramsSetColor("upper_color",mR,mG,mB);
			yI.paramsSetFloat("upper_value",0);
		}
		else
		{
			yI.paramsSetColor("upper_color",0,0,0);
			yI.paramsSetFloat("upper_value",mR);
		}

		if (texType=="yafImageTexture")
		{
			cout<<"color input true"<<endl;
			yI.paramsSetBool("color_input",true);

			texFn.findPlug("UseAlpha").getValue(texBools);
			bool calcAlpha;
			texFn.findPlug("CalculateAlpha").getValue(calcAlpha);
			yI.paramsSetBool("use_alpha",texBools||(!calcAlpha));
		}
		else
		{
			cout<<"color input false"<<endl;
			yI.paramsSetBool("color_input",false);
			yI.paramsSetBool("use_alpha",false);
		}

		//*******************************set mapper**********************************//
		yI.paramsPushList();
		yI.paramsSetString("element","shader_node");
		yI.paramsSetString("type","texture_mapper");
		yI.paramsSetString("name",mapperName.asChar());
		yI.paramsSetString("texture",texName.asChar());

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

		yI.paramsSetPoint("offset",0.0,0.0,0.0);
		yI.paramsSetPoint("scale",1.0,1.0,1.0);
		yI.paramsSetInt("proj_x",1);
		yI.paramsSetInt("proj_y",2);
		yI.paramsSetInt("proj_z",3);

		cout<<"================create layer and mapper succeeded================"<<endl;

	}

	return stat;
}
//MStatus getShader::readTexLayer(yafrayInterface_t &yI, MString layerName, std::map<string,yafaray::texture_t*> &textureMap)
//{
//	MStatus stat;
//	MSelectionList list;
//	MGlobal::getSelectionListByName(layerName,list);
//	for (unsigned int index=0; index<list.length(); index++)
//	{
//		MObject layerObject;
//		list.getDependNode(index,layerObject);
//		MFnDependencyNode layerFn(layerObject);
//
//		//find the texture connected to the first input
//		MPlug inputP1=layerFn.findPlug("LayerInput1");
//		MPlugArray srcP1;
//		inputP1.connectedTo(srcP1,true,false);
//
//		if (srcP1.length()==0)
//		{
//			cout<<"nothing connected to input 1"<<endl;
//			break;
//		}
//
//		//find the name of the input texture
//		MString nType,nName;
//		nodeType(srcP1[0],nType,nName);
//
//		//check if the input thing is a yafaray texture
//		string nodeName(nName.asChar());
//		std::map<string , yafaray::texture_t *>::iterator iter=textureMap.find(nodeName);
//		if(iter==textureMap.end())
//		{
//			cout<<nodeName<<" is not a yafaray texture"<<endl;
//			break;
//		}
//
//		cout<<"=================texture layer function test================="<<endl;
//
//
//		yI.paramsPushList();
//		yI.paramsSetString("element","shader_node");
//		yI.paramsSetString("type","layer");
//		yI.paramsSetString("name",layerName.asChar());
//		MString mapperName=nName+"Mapper";
//		yI.paramsSetString("input",mapperName.asChar());
//
//		short mixMode;
//		layerFn.findPlug("MixMethod1").getValue(mixMode);
//		yI.paramsSetInt("mode",mixMode);
//
//		//haven't added them
//		yI.paramsSetBool("stencil",false);
//		yI.paramsSetBool("negative",false);
//		yI.paramsSetBool("noRGB",false);
//		yI.paramsSetColor("def_col",1.0,0.5,0.5);
//		yI.paramsSetFloat("def_val",1.0);
//		yI.paramsSetFloat("colfac",1.0);
//		yI.paramsSetFloat("valfac",1.0);
//
//		//if (nType=="yafImageTexture")
//		//{
//			yI.paramsSetBool("color_input",false);
//		//}
//		//else
//		//{
//		//	cout<<"============texture layer test============"<<endl;
//		//	cout<<"color input false"<<endl;
//		//	yI.paramsSetBool("color_input",false);
//		//}
//
//		yI.paramsSetBool("use_alpha",false);
//		yI.paramsSetColor("upper_color",1.0,1.0,1.0);
//		yI.paramsSetFloat("upper_value",0.0);
//
//		yI.paramsSetBool("do_color",true);
//		yI.paramsSetBool("do_scalar",false);
//
//		readTexMapping(yI,nName);
//
//	}
//
//	return stat;
//}
//
//MStatus getShader::readTexMapping(yafrayInterface_t &yI, MString textureName)
//{
//	MStatus stat;
//	MSelectionList list;
//	MGlobal::getSelectionListByName(textureName,list);
//	for (unsigned int index=0; index<list.length(); index++)
//	{
//		MObject textureNode;
//		list.getDependNode(index,textureNode);
//		MFnDependencyNode texFn(textureNode);
//
//		yI.paramsPushList();
//		yI.paramsSetString("element","shader_node");
//		yI.paramsSetString("type","texture_mapper");
//		//mapper name can't be the same with texture name
//		MString mapperName=textureName+"Mapper";
//		yI.paramsSetString("name",mapperName.asChar());
//		yI.paramsSetString("texture",textureName.asChar());
//
//		short texcoMethod;
//		texFn.findPlug("MappingMethod").getValue( texcoMethod);
//		switch (texcoMethod)
//		{
//		case 0:
//			yI.paramsSetString("texco","uv");
//			break;
//		case 1:
//			yI.paramsSetString("texco","orco");
//			break;
//		case 2:
//			yI.paramsSetString("texco","global");
//			break;
//		case 3:
//			yI.paramsSetString("texco","window");
//			break;
//		}
//
//		short mapMethod;
//		texFn.findPlug("TextureCoordinate").getValue(mapMethod);
//		switch (mapMethod)
//		{
//		case 0:
//			yI.paramsSetString("mapping","plain");
//			break;
//		case 1:
//			yI.paramsSetString("mapping","cube");
//			break;
//		case 2:
//			yI.paramsSetString("mapping","tube");
//			break;
//		case 3:
//			yI.paramsSetString("mapping","sphere");
//			break;
//		}
//		yI.paramsSetPoint("offset",0.0,0.0,0.0);
//		yI.paramsSetPoint("scale",1.0,1.0,1.0);
//		yI.paramsSetInt("proj_x",1);
//		yI.paramsSetInt("proj_y",2);
//		yI.paramsSetInt("proj_z",3);
//	}
//
//	return stat;
//}