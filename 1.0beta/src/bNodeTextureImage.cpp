#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureImage.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId imageTexNode::id(0x75315);

MObject imageTexNode::imageFile;
MObject imageTexNode::imageRepeatX;
MObject imageTexNode::imagerepeatY;
MObject imageTexNode::imageUseAlpha;
MObject imageTexNode::imageCalcAlpha;
MObject imageTexNode::imageNormalMap;
MObject imageTexNode::imageMinX;
MObject imageTexNode::imageMinY;
MObject imageTexNode::imageMaxX;
MObject imageTexNode::imageMaxY;
MObject imageTexNode::mappingMethod;
MObject imageTexNode::texCo;
//texture layer settings
MObject imageTexNode::layerMix;
MObject imageTexNode::textureColor;
MObject imageTexNode::texColorFact;
MObject imageTexNode::defVal;
MObject imageTexNode::valFact;
MObject imageTexNode::doColor;
MObject imageTexNode::negative;
MObject imageTexNode::noRGB;
MObject imageTexNode::stencil;

MObject imageTexNode::UV;
MObject imageTexNode::UVFilterSize;
MObject imageTexNode::Output;

#define MAKE_INPUT(attr)                                                \
	CHECK_MSTATUS( attr.setKeyable(true) );     \
	CHECK_MSTATUS( attr.setStorable(true) );        \
	CHECK_MSTATUS( attr.setReadable(true) );    \
	CHECK_MSTATUS( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)                                               \
	CHECK_MSTATUS(attr.setKeyable(false) );     \
	CHECK_MSTATUS(attr.setStorable(false) );        \
	CHECK_MSTATUS(attr.setReadable(true) );     \
	CHECK_MSTATUS(attr.setWritable(false) );

void * imageTexNode::creator()
{
	return new imageTexNode;
}
MStatus imageTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;
	MFnTypedAttribute tAttr;

	imageFile=tAttr.create("ImageFile","if",MFnData::kString);
	MAKE_INPUT(tAttr);

	imageRepeatX=numAttr.create("RepeatX","rx",MFnNumericData::kInt,1);
	MAKE_INPUT(numAttr);
	numAttr.setMin(1);
	numAttr.setMax(512);

	imagerepeatY=numAttr.create("RepeatY","ry",MFnNumericData::kInt,1);
	MAKE_INPUT(numAttr);
	numAttr.setMin(1);
	numAttr.setMax(512);

	imageUseAlpha=numAttr.create("UseAlpha","ua",MFnNumericData::kBoolean,0);
	MAKE_INPUT(numAttr);

	imageCalcAlpha=numAttr.create("CalculateAlpha","ca",MFnNumericData::kBoolean,0);
	MAKE_INPUT(numAttr);

	imageNormalMap=numAttr.create("NormalMap","nm",MFnNumericData::kBoolean,0);
	MAKE_INPUT(numAttr);

	imageMinX=numAttr.create("MinX","minx",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMinY=numAttr.create("MinY","miny",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMaxX=numAttr.create("MaxX","maxx",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMaxY=numAttr.create("MaxY","maxy",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	mappingMethod=enumAttr.create("MappingMethod","mame",0);
	enumAttr.addField("uv",0);
	enumAttr.addField("orco",1);
	enumAttr.addField("global",2);
	enumAttr.addField("window",3);
	MAKE_INPUT(enumAttr);

	texCo=enumAttr.create("TextureCoordinate","texco",0);
	enumAttr.addField("plain",0);
	enumAttr.addField("cube",1);
	enumAttr.addField("tube",2);
	enumAttr.addField("sphere",3);
	MAKE_INPUT(enumAttr);

	//*******************************layer texture attribute*********************************//
	layerMix=enumAttr.create("MixMethod","mm1",0);
	enumAttr.addField("mix",0);
	enumAttr.addField("add",1);
	enumAttr.addField("multiply",2);
	enumAttr.addField("subtract",3);
	enumAttr.addField("screen",4);
	enumAttr.addField("divide",5);
	enumAttr.addField("difference",6);
	enumAttr.addField("darken",7);
	enumAttr.addField("lighten",8);
	MAKE_INPUT(enumAttr);

	textureColor=numAttr.createColor("TextureColor","teco");
	numAttr.setDefault(1.0,0.0,1.0);
	MAKE_INPUT(numAttr);

	texColorFact=numAttr.create("TextureColorWeight","tcw",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	MAKE_INPUT(numAttr);

	defVal=numAttr.create("DefValue","dev",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	MAKE_INPUT(numAttr);

	valFact=numAttr.create("ValueWeight","vaw",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	MAKE_INPUT(numAttr);

	doColor=numAttr.create("DoColor","doco",MFnNumericData::kBoolean,true);
	MAKE_INPUT(numAttr);

	negative=numAttr.create("Negative","nega",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

	noRGB=numAttr.create("NoRGB","nr",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

	stencil=numAttr.create("Stencil","sten",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);
	//*******************************layer texture attribute end*********************************//

	MObject u=numAttr.create("uCoord","u",MFnNumericData::kFloat);
	MObject v=numAttr.create("vCoord","v",MFnNumericData::kFloat);
	UV=numAttr.create("uvCoord","uv",u,v);
	MAKE_INPUT(numAttr);

	MObject filterX=numAttr.create("uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	MObject filterY=numAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	UVFilterSize=numAttr.create("uvFilterSize", "fs", filterX, filterY);
	MAKE_INPUT(numAttr);

	Output=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	MAKE_OUTPUT(numAttr);

	addAttribute(imageFile);
	addAttribute(imageRepeatX);
	addAttribute(imagerepeatY);
	addAttribute(imageUseAlpha);
	addAttribute(imageCalcAlpha);
	addAttribute(imageNormalMap);
	addAttribute(imageMinX);
	addAttribute(imageMinY);
	addAttribute(imageMaxX);
	addAttribute(imageMaxY);
	addAttribute(mappingMethod);
	addAttribute(texCo);

	addAttribute(layerMix);
	addAttribute(textureColor);
	addAttribute(texColorFact);
	addAttribute(defVal);
	addAttribute(valFact);
	addAttribute(doColor);
	addAttribute(negative);
	addAttribute(noRGB);
	addAttribute(stencil);

	addAttribute(UV);
	addAttribute(UVFilterSize);
	addAttribute(Output);

	attributeAffects(imageFile,Output);
	attributeAffects(imageRepeatX,Output);
	attributeAffects(imagerepeatY,Output);
	attributeAffects(imageUseAlpha,Output);
	attributeAffects(imageCalcAlpha,Output);
	attributeAffects(imageNormalMap,Output);
	attributeAffects(imageMinX,Output);
	attributeAffects(imageMinY,Output);
	attributeAffects(imageMaxX,Output);
	attributeAffects(imageMaxY,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(layerMix,Output);
	attributeAffects(textureColor,Output);
	attributeAffects(texColorFact,Output);
	attributeAffects(defVal,Output);
	attributeAffects(valFact,Output);
	attributeAffects(doColor,Output);
	attributeAffects(negative,Output);
	attributeAffects(noRGB,Output);
	attributeAffects(stencil,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;
}
MStatus imageTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=Output)&&(plug.parent() != Output))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(textureColor);
	const MFloatVector & iColor=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(Output);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=iColor;
	outColorHandle.setClean();

	return stat;

}