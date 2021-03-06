#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif
#include <iostream>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
#include <string>
#include <map>
using namespace yafaray;
using namespace std;

#ifndef RENDERPREVIEW
#define RENDERPREVIEW


class renderPreview : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new renderPreview;}
private:
	MStatus toRenderView(const int sizex, const int sizey,  const float * imageM);
	static std::map<string , yafaray::material_t*> materialMap;
	static std::map<string, yafaray::texture_t*> textureMap;
};

#endif