#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureWood.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId woodTexNode::id(0x75318);

MObject woodTexNode::woodTurbulence;
MObject woodTexNode::woodSize;
MObject woodTexNode::woodHard;
MObject woodTexNode::woodType;
MObject woodTexNode::NoiseType;
MObject woodTexNode::mappingMethod;
MObject woodTexNode::texCo;
MObject woodTexNode::UV;
MObject woodTexNode::UVFilterSize;
MObject woodTexNode::Output;

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

void* woodTexNode::creator()
{
	return new woodTexNode;
}

MStatus woodTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	woodTurbulence=numAttr.create("Turbulence","tl",MFnNumericData::kFloat,5.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(200.0);

	woodSize=numAttr.create("WoodSize","ws",MFnNumericData::kFloat,0.25);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(2.0);

	woodHard=numAttr.create("Hard","hr",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

	woodType=enumAttr.create("WoodType","wt",0);
	enumAttr.addField("bands",0);
	enumAttr.addField("rings",1);
	MAKE_INPUT(enumAttr);

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


	addAttribute(woodTurbulence);
	addAttribute(woodSize);
	addAttribute(woodHard);
	addAttribute(woodType);
	addAttribute(NoiseType);
	addAttribute(mappingMethod);
	addAttribute(texCo);
	addAttribute(UV);
	addAttribute(UVFilterSize);
	addAttribute(Output);


	attributeAffects(woodTurbulence,Output);
	attributeAffects(woodSize,Output);
	attributeAffects(woodHard,Output);
	attributeAffects(woodType,Output);
	attributeAffects(NoiseType,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;
}

MStatus woodTexNode::compute(const MPlug &plug, MDataBlock &data)
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