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

	imageMaxX=numAttr.create("MaxX","maxx",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMaxY=numAttr.create("MaxY","maxy",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	mappingMethod=enumAttr.create("MappingMethod","mame",0);
	enumAttr.addField("uv",0);
	enumAttr.addField("orco",1);
	enumAttr.addField("global",2);
	enumAttr.addField("window",3);
	MAKE_INPUT(enumAttr);

	texCo=enumAttr.create("TextureCoordinate","teco",0);
	enumAttr.addField("plain",0);
	enumAttr.addField("cube",1);
	enumAttr.addField("tube",2);
	enumAttr.addField("sphere",3);
	MAKE_INPUT(enumAttr);

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

	const MFloatVector color(0.0,0.0,0.0);

	MDataHandle outColorHandle=data.outputValue(Output);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();

	return stat;

}