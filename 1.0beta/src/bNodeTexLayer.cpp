#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTexLayer.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId texLayerNode::id(0x75316);

MObject texLayerNode::layerInput1;
MObject texLayerNode::layerInput2;
MObject texLayerNode::layerInput3;
MObject texLayerNode::layerInput4;
MObject texLayerNode::layerUV;
MObject texLayerNode::layerUVFilterSize;
MObject texLayerNode::layerOutput;

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

void * texLayerNode::creator()
{
	return new texLayerNode;
}
MStatus texLayerNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	layerInput1=numAttr.createColor("LayerInput1","li1");
	numAttr.setDefault(1.0,1.0,1.0);
	MAKE_INPUT(numAttr);

	layerInput2=numAttr.createColor("LayerInput2","li2");
	MAKE_INPUT(numAttr);
	numAttr.setDefault(1.0,1.0,1.0);

	layerInput3=numAttr.createColor("LayerInput3","li3");
	MAKE_INPUT(numAttr);
	numAttr.setDefault(1.0,1.0,1.0);

	layerInput4=numAttr.createColor("LayerInput4","li4");
	MAKE_INPUT(numAttr);
	numAttr.setDefault(1.0,1.0,1.0);

	MObject u=numAttr.create("uCoord","u",MFnNumericData::kFloat);
	MObject v=numAttr.create("vCoord","v",MFnNumericData::kFloat);
	layerUV=numAttr.create("uvCoord","uv",u,v);
	MAKE_INPUT(numAttr);

	MObject filterX=numAttr.create("uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	MObject filterY=numAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	layerUVFilterSize=numAttr.create("uvFilterSize", "fs", filterX, filterY);
	MAKE_INPUT(numAttr);

	layerOutput=numAttr.createColor("outColor","oc");
	MAKE_OUTPUT(numAttr);
	numAttr.setHidden(true);

	addAttribute(layerInput1);
	addAttribute(layerInput2);
	addAttribute(layerInput3);
	addAttribute(layerInput4);
	addAttribute(layerUV);
	addAttribute(layerUVFilterSize);
	addAttribute(layerOutput);

	attributeAffects(layerInput1,layerOutput);
	attributeAffects(layerInput2,layerOutput);
	attributeAffects(layerInput3,layerOutput);
	attributeAffects(layerInput4,layerOutput);
	attributeAffects(layerUV,layerOutput);
	attributeAffects(layerUVFilterSize,layerOutput);

	return stat;
}
MStatus texLayerNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat;

	if ((plug !=layerOutput)&&(plug.parent() != layerOutput))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(layerInput1);
	const MFloatVector & color=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(layerOutput);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();
	return stat;
}