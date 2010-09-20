#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif
#include <iostream>
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
		MStatus getAnimationInfo(int &start, int &end, MString &format);
	private:
		MStatus setGammaInput(yafrayInterface_t &yI, MFnDependencyNode &renderFn);
		MStatus readIntegrator(yafrayInterface_t &yI , MFnDependencyNode &renderFn);
		MStatus readRender(yafrayInterface_t &yI, MFnDependencyNode &renderFn);
		MStatus readOutput(yafrayInterface_t &yI, MFnDependencyNode &renderFn);

};
#endif