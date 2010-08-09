#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef CLOUDTEX
#define CLOUDTEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class cloudTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject cloudSize;
    static MObject cloudHard;
	static MObject cloudDepth;
	static MObject NoiseType;
	static MObject mappingMethod;
	static MObject texCo;
	static MObject UV;
	static MObject UVFilterSize;
	static MObject Output;

};
#endif