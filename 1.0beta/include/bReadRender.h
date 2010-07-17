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
		MStatus readRender(yafrayInterface_t &yI);
	private:
		MStatus readIntegrator(yafrayInterface_t &yI , MFnDependencyNode &renderFn);
//		MStatus readGeneral(yafrayInterface_t &yI ,MFnDependencyNode &renderFn);
		MStatus readOutput(yafrayInterface_t &yI, MFnDependencyNode &renderFn);
		MStatus readAA(yafrayInterface_t &yI, MFnDependencyNode &renderFn);

};
#endif