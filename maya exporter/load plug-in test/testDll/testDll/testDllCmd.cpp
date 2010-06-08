//
// Copyright (C) testDll
// 
// File: testDllCmd.cpp
//
// MEL Command: testDll
//
// Author: Maya Plug-in Wizard 2.0
//

// Includes everything needed to register a simple MEL command with Maya.
// 
#include<maya/MGlobal.h>
#include<maya/MPxCommand.h>
#include<maya/MFnPlugin.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;

// Use helper macro to register a command with Maya.  It creates and
// registers a command that does not support undo or redo.  The 
// created class derives off of MPxCommand.
//
class testDllCmd: public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new testDllCmd;}
};


MStatus testDllCmd::doIt( const MArgList& args )

{
	MStatus stat = MS::kSuccess;
	yafrayInterface_t test;

	MString loadDll("getenv ""YAFARAY_PLUG_IN_PATH""");
	MString loadDllResult;
	MGlobal::executeCommand(loadDll,loadDllResult);

	test.loadPlugins("F:/arioso_works/GSOC/Process/dlls");
	MGlobal::displayInfo("load plug-in succeeded!");


	return stat;
}

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj,"YafaRay","2011","Any");
	stat=plugin.registerCommand("testDll",testDllCmd::creator);

	if(!stat) stat.perror("register command failed T_T");

	stat=plugin.registerUI("testDllCreateUI","testDllDeleteUI");
	if(!stat) stat.perror("damn, where is it?");

	//MStringArray testDllMenuFeedback;
	//testDllMenuFeedback=plugin.addMenuItem("testDllMenu","testwindow|menu39","testDll","");
	
	

	return stat;
}
MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj);
	stat=plugin.deregisterCommand("testDll");
	if(!stat) stat.perror("damn, deregister failed");

	//MStringArray testDllMenu;
	//testDllMenu.append("testDllMenu");
	//stat=plugin.removeMenuItem(testDllMenu);
	//if(!stat) stat.perror("remove menu failed!");

	return MS::kSuccess;
}