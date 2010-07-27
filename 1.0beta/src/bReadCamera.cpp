#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bReadCamera.h"
#include "bReadRender.h"

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

	//test output
	MFnDagNode cameraFn(cameraPath);
	MGlobal::displayInfo(cameraFn.name());
	

	MFnCamera currentCam(cameraPath);
	MVector eyePoint=currentCam.eyePoint(MSpace::kWorld);
	//test output
	cout<<eyePoint.x<<eyePoint.y<<eyePoint.z<<endl;

	MVector to=currentCam.viewDirection(MSpace::kWorld);
	//test output
	cout<<to.x<<to.y<<to.z<<endl;

	MVector up=currentCam.upDirection(MSpace::kWorld);
	//test output
	cout<<up.x<<up.y<<up.z<<endl;

	//try to find resx, resy, even i don't know what do they mean......
	MRenderData rData;
	int resx=rData.resX;
	int resy=rData.resY;


	yI.paramsClearAll();
	yI.paramsSetString("type","perspective");
	yI.paramsSetPoint("from",eyePoint.x, eyePoint.y, eyePoint.z);
	yI.paramsSetPoint("up",up.x,up.y,up.z);
	yI.paramsSetPoint("to",to.x,to.y,to.z);
	yI.paramsSetFloat("focal", 1.0);

	//cant find the damn size data of the rendering image...so find a temp way
	getRender renderData;
	int sizex, sizey;
	renderData.getImageWidth(sizex);
	renderData.getImageHeight(sizey);

	yI.paramsSetInt("resx",sizex);
	yI.paramsSetInt("resy",sizey);
	yI.createCamera("cam");

	return stat=MStatus::kSuccess;
}