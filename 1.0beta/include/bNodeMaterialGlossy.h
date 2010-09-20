#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifndef GLOSSYNODE
#define GLOSSYNODE

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif

#include <iostream>
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class glossyNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:

	static MObject diffuseTexLayer;
	static MObject glossyTexLayer;
	static MObject glossyReTexLayer;
	static MObject bumpTexLayer;

	static MObject diffuseColor;
	static MObject diffuseReflect;
	static MObject glossyColor;
	static MObject glossyReflect;
	static MObject exponent;
	static MObject asDiffuse;
	static MObject anisotropic;
	static MObject expHorizontal;
	static MObject expVertical;
	static MObject outGlossy;
};

#endif