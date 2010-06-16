
#include<maya/MPxCommand.h>

#ifndef TEST
#define TEST

class testSelectAddAttribute: public MPxCommand
{
public:
	virtual MStatus doIt( const MArgList& args );;
	static void *creator(){return new testSelectAddAttribute;}
};

#endif