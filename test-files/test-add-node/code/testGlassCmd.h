#include<maya/MPxCommand.h>

#ifndef TESTGLASSNODE
#define TESTGLASSNODE
class testGlassCmd: public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new testGlassCmd;}

};

#endif