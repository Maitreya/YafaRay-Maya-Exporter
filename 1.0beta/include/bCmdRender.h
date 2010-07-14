#include<map>
#include<string>
#include<maya/MPxCommand.h>
#include<maya/MGlobal.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;
using namespace std;

#ifndef RENDERSCENE
#define RENDERSCENE

class renderScene:public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new renderScene;}
	MStatus beginRender();
private:
	yafrayInterface_t yI;
	std::map<const char *,yafaray::material_t*> materialMap;
	
};
#endif
