#include"bReadCamera.h"

#include<maya/M3dView.h>
#include<maya/MDagPath.h>
#include<maya/MFnDagNode.h>
#include<maya/MFnTransform.h>
#include<maya/MVector.h>
#include<maya/MFnCamera.h>
#include<maya/MRenderData.h>
#include<iostream>

MStatus getCamera::readCamera(yafaray::yafrayInterface_t &yI)
{
	MStatus stat;

	//get camera of the current view and read information of it
	M3dView currentView=M3dView::active3dView();
	MDagPath cameraPath;
	currentView.getCamera(cameraPath);
	MObject cameraTransNode=cameraPath.transform();
	MFnTransform cameraTrans(cameraTransNode);
	MVector cameraP=cameraTrans.getTranslation(MSpace::kTransform);

	//test output
	cout<<cameraP.x<<cameraP.y<<cameraP.z<<endl;

	//test output
	MFnDagNode cameraFn(cameraPath);
	MGlobal::displayInfo(cameraFn.name());
	

	MFnCamera currentCam(cameraPath);
	MVector to=currentCam.viewDirection(MSpace::kWorld);
	//test output
	cout<<to.x<<to.y<<to.z<<endl;

	MVector up=currentCam.upDirection(MSpace::kWorld);
	//test output
	cout<<up.x<<up.y<<up.z<<endl;


	yI.paramsClearAll();
	yI.paramsSetString("type","perspective");
	yI.paramsSetPoint("from",cameraP.x,cameraP.y,cameraP.z);
	yI.paramsSetPoint("up",up.x,up.y,up.z);
	yI.paramsSetPoint("to",to.x,to.y,to.z);

	//cant find the damn size data of the rendering image...so find a temp way
	yI.paramsSetInt("resx",640);
	yI.paramsSetInt("resy",480);
	yI.createCamera("cam");

	return stat=MStatus::kSuccess;
}