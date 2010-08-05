#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bCmdLoadPlugin.h"
#include "bCmdRender.h"

#include<maya/MGlobal.h>
#include<maya/MString.h>
#include<maya/MStringArray.h>
#include<windows.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;

MStatus loadPlugin::doIt(const MArgList &args)
{
	MStatus stat=MStatus::kSuccess;

    MString getMayaLocation("getenv MAYA_LOCATION");
	MString locationPath;
	MGlobal::executeCommand(getMayaLocation,locationPath);
	MStringArray extraDlls;
	extraDlls.append("libpng14d");
	extraDlls.append("libtiff3");
	extraDlls.append("libxml2");
	extraDlls.append("yafaraycore");
	extraDlls.append("yafarayqt");
	extraDlls.append("yafarayplugin");
	extraDlls.append("zlib");
	MString pathAdded("/bin");
	MString suffix(".dll");
	for(unsigned int i=0; i<extraDlls.length();i++)
	{
		MString dll=locationPath+pathAdded+extraDlls[i]+suffix;
		cout<<"loading dll "<<extraDlls[i]<<".dll";
		LoadLibrary(dll.asChar());
		cout<<"........................succeeded!"<<endl;
	}


	MString yafPath("/bin/plug-ins/yafaray");
	MString yafDll=locationPath+yafPath;
    yafrayInterface_t * yafLoad=renderScene::getyI();
	yafLoad->loadPlugins(yafDll.asChar());
	MGlobal::displayInfo("load yafaray components succeeded!");


	return stat;
}