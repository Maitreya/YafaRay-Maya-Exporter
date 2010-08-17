#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef MARBLETEX
#define MARBLETEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class marbleTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	//shape not needed to set
	static MObject marbleDepth;
	static MObject marbleTurbulence;
	static MObject marbleSize;
	static MObject marbleHard;
	//can't find this
	//static MObject marbleSharpness;
	static MObject NoiseType;
	static MObject mappingMethod;
	static MObject texCo;
	//layer settings
	//colorInput, upperColor, upperValue, useAlpha ,doScale, don't need settings
	static MObject layerMix;
	static MObject textureColor;
	static MObject texColorFact;
	static MObject defVal;
	static MObject valFact;
	static MObject doColor;
	static MObject negative;
	static MObject noRGB;
	static MObject stencil;

	static MObject UV;
	static MObject UVFilterSize;
	static MObject Output;

};
#endif