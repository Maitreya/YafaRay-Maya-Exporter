#include<maya/MPxCommand.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;

#ifndef READOBJECT
#define READOBJECT

class readObject: public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new readObject;}
private:
   MStatus readMesh(void);
   MStatus readPartical(void);
   yafrayInterface_t yI;

};

#endif