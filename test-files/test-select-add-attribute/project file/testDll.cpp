#include<maya/MGlobal.h>
#include<maya/MPxCommand.h>
#include<interface/yafrayinterface.h>
#include"testDll.h"

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
