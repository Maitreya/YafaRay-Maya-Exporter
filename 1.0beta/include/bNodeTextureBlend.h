#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef BLENDTEX
#define BLENDTEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class blendTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject blendStype;
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

	static MObject blendUV;
	static MObject blendUVFilterSize;
	static MObject blendOutput;

};
#endif