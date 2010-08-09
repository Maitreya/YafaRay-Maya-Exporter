#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef DISNOISETEX
#define DISNOISETEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class disnoiseTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject disnoiseDistort;
	static MObject disnoiseSize;

	static MObject NoiseType1;
	static MObject NoiseType2;
	static MObject mappingMethod;
	static MObject texCo;
	static MObject UV;
	static MObject UVFilterSize;
	static MObject Output;

};
#endif