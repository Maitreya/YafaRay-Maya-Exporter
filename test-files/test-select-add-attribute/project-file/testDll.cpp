#include<maya/MGlobal.h>
#include<maya/MPxCommand.h>
#include<interface/yafrayinterface.h>
#include"testDll.h"

using namespace yafaray;





MStatus testDllCmd::doIt( const MArgList& args )

{
	MStatus stat = MS::kSuccess;
	

	/*MString loadDll("getenv ""YAFARAY_PLUG_IN_PATH""");
	MString loadDllResult;
	MGlobal::executeCommand(loadDll,loadDllResult);*/
	std::string dllPath="F:/arioso_works/GSOC/Process/extradlls";
	//const char *dll=dllPath.asChar();
	std::string extraDlls[]={"Half","Iex","IlmImf","IlmThread","jpeg62","libfreetype-6","libpng3","mingwm10","yafaraycore","yafarayplugin","zlib1"};
	std::string suffix=".dll";
	for(int i=0;i<11;i++)
	{
		cout<<"loading dll "<<extraDlls[i]<<".dll";
		LoadLibrary((dllPath+extraDlls[i]+suffix).c_str());
		cout<<"......succeeded!"<<endl;
		
		
	
	}


    yafrayInterface_t test;
	test.loadPlugins("F:/arioso_works/GSOC/Process/newdlls");
	MGlobal::displayInfo("load plug-in succeeded!");


	return stat;
}
