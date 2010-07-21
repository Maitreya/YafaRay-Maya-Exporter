#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bReadLight.h"

#include<maya/MGlobal.h>
#include<maya/MSelectionList.h>
#include<maya/MPlug.h>
#include<maya/MPlugArray.h>
#include<maya/MDagPath.h>
#include<maya/MFnDagNode.h>
#include<maya/MMatrix.h>
#include<maya/MFnTransform.h>
#include<maya/MVector.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MFnNumericData.h>
#include<iostream>
#include<interface/yafrayinterface.h>
using namespace yafaray;

MStatus getLight::readLight(yafaray::yafrayInterface_t &yI)
{
	MStatus stat;

	MString listPoint("ls -type yafPointLight");
	MStringArray pointResult;
	MGlobal::executeCommand(listPoint, pointResult);
	for(unsigned int i=0;i<pointResult.length();i++)
	{
		//test output
		cout<<pointResult[i].asChar()<<endl;

		MSelectionList list;
		MGlobal::getSelectionListByName(pointResult[i],list);
		for(unsigned int index=0;index<list.length();index++)
		{
		    MDagPath pointPath;
		    list.getDagPath(index,pointPath);
		
		    //test output
		    MGlobal::displayInfo(pointPath.fullPathName());

		    MObject pointTransNode=pointPath.transform();
		    MFnTransform pointTrans(pointTransNode);
		    MGlobal::displayInfo(pointTrans.name());
		    MVector transP=pointTrans.getTranslation(MSpace::kTransform);

		    //test output
			//this is the position of the point light
		    cout<<transP.x<<transP.y<<transP.z<<endl;

			MObject pointDepNode;
			list.getDependNode(index,pointDepNode);
			MFnDependencyNode pointDepFn(pointDepNode);

			yI.paramsClearAll();
			yI.paramsSetString("type","pointlight");

			yI.paramsSetPoint("from",transP.x,transP.y,transP.z);

			MObject pointColor;
			pointDepFn.findPlug("LightColor").getValue(pointColor);
			MFnNumericData pointData(pointColor);
			float pcR,pcG,pcB;
			pointData.getData(pcR,pcG,pcB);
			yI.paramsSetColor("color",pcR,pcG,pcB);

			double pointPower;
			pointDepFn.findPlug("Power").getValue(pointPower);
			yI.paramsSetFloat("power",pointPower);

			yI.createLight(pointDepFn.name().asChar());

		}

	}

	return stat;
}