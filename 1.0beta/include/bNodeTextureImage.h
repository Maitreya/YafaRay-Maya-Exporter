#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef IMAGETEX
#define IMAGETEX

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class imageTexNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject imageFile;
	static MObject imageRepeatX;
	static MObject imagerepeatY;
	static MObject imageUseAlpha;
	static MObject imageCalcAlpha;
	static MObject imageNormalMap;
//	static MObject imageExtend; this is a very blender setting, i think
	static MObject imageMinX;
	static MObject imageMinY;
	static MObject imageMaxX;
	static MObject imageMaxY;
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