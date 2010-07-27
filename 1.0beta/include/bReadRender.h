#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;

#ifndef READRENDER
#define READRENDER
class getRender
{
	//export integrator, volume and general render settings
	public:
		MStatus createRender(yafrayInterface_t &yI);
		MStatus getImageWidth(int &width);
		MStatus getImageHeight(int &height);
	private:
		MStatus setGammaInput(yafrayInterface_t &yI, MFnDependencyNode &renderFn);
		MStatus readIntegrator(yafrayInterface_t &yI , MFnDependencyNode &renderFn);
		MStatus readRender(yafrayInterface_t &yI, MFnDependencyNode &renderFn);
		MStatus readOutput(yafrayInterface_t &yI, MFnDependencyNode &renderFn);

};
#endif