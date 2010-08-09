#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureBlend.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId blendTexNode::id(0x75317);

MObject blendTexNode::blendStype;
MObject blendTexNode::mappingMethod;
MObject blendTexNode::texCo;
MObject blendTexNode::blendUV;
MObject blendTexNode::blendUVFilterSize;
MObject blendTexNode::blendOutput;

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

void* blendTexNode::creator()
{
	return new blendTexNode;
}

MStatus blendTexNode::initialize()
{
	MStatus stat;
	MFnEnumAttribute enumAttr;
	MFnNumericAttribute numAttr;

	blendStype=enumAttr.create("Stype","st",0);
	enumAttr.addField("lin",0);
	enumAttr.addField("quad",1);
	enumAttr.addField("ease",2);
	enumAttr.addField("diag",3);
	enumAttr.addField("sphere",4);
	enumAttr.addField("halo",5);
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
	blendUV=numAttr.create("uvCoord","uv",u,v);
	MAKE_INPUT(numAttr);

	MObject filterX=numAttr.create("uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	MObject filterY=numAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	blendUVFilterSize=numAttr.create("uvFilterSize", "fs", filterX, filterY);
	MAKE_INPUT(numAttr);

	blendOutput=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	MAKE_OUTPUT(numAttr);

	addAttribute(blendStype);
	addAttribute(mappingMethod);
	addAttribute(texCo);
	addAttribute(blendUV);
	addAttribute(blendUVFilterSize);
	addAttribute(blendOutput);

	attributeAffects(blendStype,blendOutput);
	attributeAffects(mappingMethod,blendOutput);
	attributeAffects(texCo,blendOutput);
	attributeAffects(blendUV,blendOutput);
	attributeAffects(texCo,blendOutput);

	return stat;
}
MStatus blendTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=blendOutput)&&(plug.parent() !=blendOutput))
	{
		return MStatus::kUnknownParameter;
	}

	const MFloatVector color(0.0,0.0,0.0);

	MDataHandle outColorHandle=data.outputValue(blendOutput);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();
	return stat;
}