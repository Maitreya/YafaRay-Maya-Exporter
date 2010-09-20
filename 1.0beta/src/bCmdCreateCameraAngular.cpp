#define NOMINMAX
#define _USE_MATH_DEFINES 1

#include "bNodeCameraAngular.h"
#include "bCmdCreateCameraAngular.h"

#include<maya/MGlobal.h>
#include<maya/MString.h>
#include<maya/MSelectionList.h>
#include<maya/MItSelectionList.h>
#include<maya/MDGModifier.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MFnNumericAttribute.h>
#include<maya/MPlug.h>
#include<maya/MPlugArray.h>



MStatus createAngular::doIt(const MArgList &args)
{
	MStatus stat;

	MSelectionList list;
	MFnDependencyNode angularDepFn;
	MDGModifier dgMod;

	MObject angularCamera=dgMod.createNode(angularNode::id);
	angularDepFn.setObject(angularCamera);

	MGlobal::getActiveSelectionList(list);
	MItSelectionList cameraIter(list,MFn::kCamera);
	for(; !cameraIter.isDone(); cameraIter.next())
	{
		MObject cameraNode;
		cameraIter.getDependNode(cameraNode);

	    MFnDependencyNode cameraDepFn;
		cameraDepFn.setObject(cameraNode);

		if(!cameraDepFn.hasAttribute("YafarayCamera"))
		{
			MFnNumericAttribute numAttr;
			MObject attrYafarayCamera=numAttr.create("YafarayCamera","yaca",MFnNumericData::kBoolean,0);
			cameraDepFn.addAttribute(attrYafarayCamera,MFnDependencyNode::kLocalDynamicAttr);
		}

		MPlug angularPlug=angularDepFn.findPlug("OutAngular");
		MPlug cameraPlug=cameraDepFn.findPlug("YafarayCamera");

		MPlugArray srcCameraPlug;
		if(cameraPlug.connectedTo(srcCameraPlug,true,false))
		{
			MPlug originYafarayCameraPlug=srcCameraPlug[0];
			dgMod.disconnect(originYafarayCameraPlug,cameraPlug);
		}
		dgMod.connect(angularPlug,cameraPlug);
		MGlobal::displayInfo("Angular camera is created!");

	}

	dgMod.doIt();
	

	return stat=MStatus::kSuccess;

}