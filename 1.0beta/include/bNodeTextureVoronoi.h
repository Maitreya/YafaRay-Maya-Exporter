#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef VORONOITEX
#define VORONOITEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class voronoiTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject voronoiColorType;
	static MObject voronoiWeight1;
	static MObject voronoiWeight2;
	static MObject voronoiWeight3;
	static MObject voronoiWeight4;
	static MObject voronoiExponent;
	static MObject voronoiIntensity;
	static MObject voronoiSize;
	static MObject voronoiDistanceMetric;

	static MObject NoiseType;
	static MObject mappingMethod;
	static MObject texCo;
	static MObject UV;
	static MObject UVFilterSize;
	static MObject Output;

};
#endif