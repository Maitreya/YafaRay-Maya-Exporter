#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;

#ifndef READWORLD
#define READWORLD

class getWorld
{
public:
	MStatus readWorld(yafrayInterface_t &yI);
private:
	MStatus readBackground(yafrayInterface_t &yI , MFnDependencyNode &worldFn);
	MStatus readVolumeIntegrator(yafrayInterface_t &yI , MFnDependencyNode &worldFn);
};

#endif