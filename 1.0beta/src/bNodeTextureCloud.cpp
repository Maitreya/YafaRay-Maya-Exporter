#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureCloud.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId cloudTexNode::id(0x75318);

MObject cloudTexNode::cloudSize;
MObject cloudTexNode::cloudHard;
MObject cloudTexNode::cloudDepth;
MObject cloudTexNode::NoiseType;
MObject cloudTexNode::mappingMethod;
MObject cloudTexNode::texCo;
MObject cloudTexNode::UV;
MObject cloudTexNode::UVFilterSize;
MObject cloudTexNode::Output;

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

void* cloudTexNode::creator()
{
	return new cloudTexNode;
}

MStatus cloudTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	cloudSize=numAttr.create("CloudSize","cs",MFnNumericData::kFloat,0.25);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(2.0);

	cloudHard=numAttr.create("Hard","hr",MFnNumericData::kBoolean,0);
	MAKE_INPUT(numAttr);

	cloudDepth=numAttr.create("CloudDepth","cd",MFnNumericData::kInt,2);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0);
	numAttr.setMax(6);

	NoiseType=enumAttr.create("NoiseType","noty",0);
	enumAttr.addField("newperlin",0);
	enumAttr.addField("stdperlin",1);
	enumAttr.addField("voronoi_f1",2);
	enumAttr.addField("voronoi_f2",3);
	enumAttr.addField("voronoi_f3",4);
	enumAttr.addField("voronoi_f4",5);
	enumAttr.addField("voronoi_f2fl",6);
	enumAttr.addField("voronoi_crackle",7);
	enumAttr.addField("cellnoise",8);
	MAKE_INPUT(enumAttr);

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

	addAttribute(cloudSize);
	addAttribute(cloudHard);
	addAttribute(cloudDepth);
	addAttribute(NoiseType);
	addAttribute(mappingMethod);
	addAttribute(texCo);
	addAttribute(UV);
	addAttribute(UVFilterSize);
	addAttribute(Output);

	attributeAffects(cloudSize,Output);
	attributeAffects(cloudHard,Output);
	attributeAffects(cloudDepth,Output);
	attributeAffects(NoiseType,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;
}

MStatus cloudTexNode::compute(const MPlug &plug, MDataBlock &data)
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