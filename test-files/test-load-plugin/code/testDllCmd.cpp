

#include<maya/MGlobal.h>
#include<maya/MPxCommand.h>
#include<maya/MFnPlugin.h>
#include<interface/yafrayinterface.h>
#include<testSelectAddAttributes.h>

using namespace yafaray;





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

	if(!stat) stat.perror("register command testDll failed T_T");

	stat=plugin.registerUI("testDllCreateUI","testDllDeleteUI");
	if(!stat) stat.perror("damn, where is it?");

	stat=plugin.registerCommand("testSelectAddAttribute",testSelectAddAttribute::creator);

	if(!stat) stat.perror("register command testSelectAddAttribute failed T_T");
	//MStringArray testDllMenuFeedback;
	//testDllMenuFeedback=plugin.addMenuItem("testDllMenu","testwindow|menu39","testDll","");
	
	

	return stat;
}
MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj);
	stat=plugin.deregisterCommand("testDll");
	if(!stat) stat.perror("damn, deregister testDll failed");

	stat=plugin.deregisterCommand("testSelectAddAttribute");
	if(!stat) stat.perror("damn, deregister testSelectAddAttribute failed");

	//MStringArray testDllMenu;
	//testDllMenu.append("testDllMenu");
	//stat=plugin.removeMenuItem(testDllMenu);
	//if(!stat) stat.perror("remove menu failed!");

	return MS::kSuccess;
}