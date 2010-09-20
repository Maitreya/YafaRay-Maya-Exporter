#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif

#ifndef TEXLAYER
#define TEXLAYER
#include <iostream>
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class texLayerNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:

	static MObject layerInput1;
	static MObject layerInput2;
	static MObject layerInput3;
	static MObject layerInput4;
	static MObject layerUV;
	static MObject layerUVFilterSize;
	static MObject layerOutput;

};
#endif