#include "bCmdLoadPlugin.h"

#include<maya/MGlobal.h>
#include<maya/MString.h>
#include<maya/MStringArray.h>
#include<windows.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;

MStatus loadPlugin::doIt(const MArgList &args)
{
	MStatus stat=MStatus::kSuccess;

    MString getExtraPath("getenv ""YAFARAY_EXTRADLL_PATH""");
	MString extraPath;
	MGlobal::executeCommand(getExtraPath,extraPath);
	MStringArray extraDlls;
	extraDlls.append("libpng14d");
	extraDlls.append("libtiff3");
	extraDlls.append("libxml2");
	extraDlls.append("yafaraycore");
	extraDlls.append("yafarayqt");
	extraDlls.append("yafarayplugin");
	extraDlls.append("zlib");
	MString suffix(".dll");
	for(unsigned int i=0; i<extraDlls.length();i++)
	{
		MString dll=extraPath+extraDlls[i]+suffix;
		cout<<"loading dll "<<extraDlls[i]<<".dll";
		LoadLibrary(dll.asChar());
		cout<<"........................succeeded!"<<endl;
	}

	
	MString getPath("getenv ""YAFARAY_PLUG_IN_PATH""");
	MString pluginPath;
	MGlobal::executeCommand(getPath,pluginPath);

	yafrayInterface_t yafLoad;
	yafLoad.loadPlugins(pluginPath.asChar());
	MGlobal::displayInfo("load yafaray components succeeded!");


	return stat;
}