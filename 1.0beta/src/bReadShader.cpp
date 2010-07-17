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
			cout<<gName<<endl;
			materialMap[gName]=glassMat;



		}

		MGlobal::displayInfo("let me see if this reading shader part works");
	}

	//then deal with other materials, but not implemented yet


	return stat;
}