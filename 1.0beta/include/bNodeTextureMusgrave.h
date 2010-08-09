#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef MUSGRAVETEX
#define MUSGRAVETEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class musgraveTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:

	static MObject musgraveType;
	static MObject musgraveH;
	static MObject musgraveLacunarity;
	static MObject musgraveOctaves;
	static MObject musgraveSize;
	static MObject musgraveIntensity;

	static MObject NoiseType;
	static MObject mappingMethod;
	static MObject texCo;
	static MObject UV;
	static MObject UVFilterSize;
	static MObject Output;

};
#endif