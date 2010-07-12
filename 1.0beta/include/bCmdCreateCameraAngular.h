#include<maya/MPxCommand.h>

#ifndef CREATEANGULAR
#define CREATEANGULAR

class createAngular : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new createAngular;}
};
#endif
