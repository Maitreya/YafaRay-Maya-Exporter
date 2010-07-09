#include<maya/MPxCommand.h>

#ifndef LOADPLUGIN
#define LOADPLUGIN

class loadPlugin: public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new loadPlugin;}
};

#endif