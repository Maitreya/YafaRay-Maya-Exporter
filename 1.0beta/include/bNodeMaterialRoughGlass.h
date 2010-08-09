#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef ROUGHGLASSNODE
#define ROUGHGLASSNODE

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class roughGlassNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject texLayer;
	static MObject exponent;
	static MObject alpha;
	static MObject absorbColor;
	static MObject absorbDistance;
	static MObject filterColor;
	static MObject mirrorColor;
	static MObject IOR;
	static MObject transmitFilter;
	static MObject dispersionPower;
	static MObject fakeShadows;
	static MObject outRoughGlass;


};
#endif