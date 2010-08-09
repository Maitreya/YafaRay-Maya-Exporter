#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureMarble.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId marbleTexNode::id(0x75319);

MObject marbleTexNode::marbleDepth;
MObject marbleTexNode::marbleTurbulence;
MObject marbleTexNode::marbleSize;
MObject marbleTexNode::marbleHard;
//can't find this
//MObject marbleTexNode::marbleSharpness;
MObject marbleTexNode::NoiseType;
MObject marbleTexNode::mappingMethod;
MObject marbleTexNode::texCo;
MObject marbleTexNode::UV;
MObject marbleTexNode::UVFilterSize;
MObject marbleTexNode::Output;

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

void* marbleTexNode::creator()
{
	return new marbleTexNode;
}

MStatus marbleTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	marbleDepth=numAttr.create("Depth","dp",MFnNumericData::kInt,2);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0);
	numAttr.setMax(6);

	marbleTurbulence=numAttr.create("Turbulence","tl",MFnNumericData::kFloat,5.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(200.0);

	marbleSize=numAttr.create("MarbleSize","ws",MFnNumericData::kFloat,0.25);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(2.0);

	marbleHard=numAttr.create("Hard","hr",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

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

    addAttribute(marbleDepth);
	addAttribute(marbleTurbulence);
	addAttribute(marbleSize);
	addAttribute(marbleHard);
	addAttribute(NoiseType);
	addAttribute(mappingMethod);
	addAttribute(texCo);
	addAttribute(UV);
	addAttribute(UVFilterSize);
	addAttribute(Output);


	attributeAffects(marbleDepth,Output);
	attributeAffects(marbleTurbulence,Output);
	attributeAffects(marbleSize,Output);
	attributeAffects(marbleHard,Output);
	attributeAffects(NoiseType,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;
}
MStatus marbleTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=Output)&&(plug.parent() !=Output))
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