#include<maya/MPxCommand.h>

#ifndef TESTSELECT
#define TESTSELECT

class testSelectAddAttribute:MPxCommand
{
public:
	virtual MStatus doIt(const MArgList &args);
	static void *creator() {return new testSelectAddAttribute;}

};

#endif