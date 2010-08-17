#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef SHINYDIFFUSENODE
#define SHINYDIFFUSENODE

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class shinyDiffuseNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
//	static MObject tString;

	static MObject colorTexLayer;
	static MObject mirrorTexLayer;
	static MObject bumpTexLayer;

	static MObject color;
	static MObject mirrorColor;
	static MObject diffuseReflection;
	static MObject mirrorStrength;
	static MObject transparency;
	static MObject translucency;
	static MObject transmitFilter;
	static MObject emit;
	static MObject fresnel;
	static MObject IOR;
	static MObject outShinyDiffuse;

};
#endif