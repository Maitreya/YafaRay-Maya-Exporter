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
//texture layer settings
MObject blendTexNode::layerMix;
MObject blendTexNode::textureColor;
MObject blendTexNode::texColorFact;
MObject blendTexNode::defVal;
MObject blendTexNode::valFact;
MObject blendTexNode::doColor;
MObject blendTexNode::negative;
MObject blendTexNode::noRGB;
MObject blendTexNode::stencil;

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

	addAttribute(layerMix);
	addAttribute(textureColor);
	addAttribute(texColorFact);
	addAttribute(defVal);
	addAttribute(valFact);
	addAttribute(doColor);
	addAttribute(negative);
	addAttribute(noRGB);
	addAttribute(stencil);

	addAttribute(blendUV);
	addAttribute(blendUVFilterSize);
	addAttribute(blendOutput);

	attributeAffects(blendStype,blendOutput);
	attributeAffects(mappingMethod,blendOutput);
	attributeAffects(texCo,blendOutput);

	attributeAffects(layerMix,blendOutput);
	attributeAffects(textureColor,blendOutput);
	attributeAffects(texColorFact,blendOutput);
	attributeAffects(defVal,blendOutput);
	attributeAffects(valFact,blendOutput);
	attributeAffects(doColor,blendOutput);
	attributeAffects(negative,blendOutput);
	attributeAffects(noRGB,blendOutput);
	attributeAffects(stencil,blendOutput);

	attributeAffects(blendUV,blendOutput);
	attributeAffects(texCo,blendOutput);

	return stat;
}
MStatus blendTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=blendOutput)&&(plug.parent() != blendOutput))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(textureColor);
	const MFloatVector & iColor=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(blendOutput);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=iColor;
	outColorHandle.setClean();
	return stat;
}