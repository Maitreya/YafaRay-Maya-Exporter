#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeMaterialShinyDiffuse.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId shinyDiffuseNode::id(0x75306);

//MObject shinyDiffuseNode::tString;
MObject shinyDiffuseNode::color;
MObject shinyDiffuseNode::colorTexLayer;
MObject shinyDiffuseNode::mirrorColor;
MObject shinyDiffuseNode::mirrorTexLayer;
MObject shinyDiffuseNode::bumpTexLayer;
MObject shinyDiffuseNode::diffuseReflection;
MObject shinyDiffuseNode::mirrorStrength;
MObject shinyDiffuseNode::transparency;
MObject shinyDiffuseNode::translucency;
MObject shinyDiffuseNode::transmitFilter;
MObject shinyDiffuseNode::emit;
MObject shinyDiffuseNode::fresnel;
MObject shinyDiffuseNode::IOR;
MObject shinyDiffuseNode::outShinyDiffuse;

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

void * shinyDiffuseNode::creator()
{
	return new shinyDiffuseNode;
}

MStatus shinyDiffuseNode::initialize()
{
	MStatus stat=MStatus::kSuccess;
	MFnNumericAttribute numAttr;
//	MFnTypedAttribute tAttr;

	//tString=tAttr.create("testString","ts",MFnData::kString);
	//numAttr.setKeyable(true);
	//numAttr.setStorable(true);

	colorTexLayer=numAttr.createColor("ColorTextureLayer","ctl"); 	MAKE_INPUT(numAttr);
	mirrorTexLayer=numAttr.createColor("MirrorTextureLayer","mtl"); 	MAKE_INPUT(numAttr);
	bumpTexLayer=numAttr.createColor("BumpTextureLayer","btl"); 	MAKE_INPUT(numAttr);

	color=numAttr.createColor("Color","dco");
	MAKE_INPUT(numAttr);
	numAttr.setDefault(1.0,1.0,1.0);

	mirrorColor=numAttr.createColor("MirrorColor","dmico");
	MAKE_INPUT(numAttr);
	numAttr.setDefault(1.0,1.0,1.0);

	diffuseReflection=numAttr.create("DiffuseReflection","ddire",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	mirrorStrength=numAttr.create("MirrorStrength","dmist",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	transparency=numAttr.create("Transparency","dtr",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	translucency=numAttr.create("Translucency","dtra",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	transmitFilter=numAttr.create("TransmitFilter","dtrfi",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	emit=numAttr.create("Emit","de",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1000.0f);

	fresnel=numAttr.create("Fresnel","dfr",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

	IOR=numAttr.create("IOR","dior",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(1.0f);
	numAttr.setMax(30.0f);

	outShinyDiffuse=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	MAKE_OUTPUT(numAttr);

//	addAttribute(tString);
	addAttribute(color);
	addAttribute(colorTexLayer);
	addAttribute(mirrorColor);
	addAttribute(mirrorTexLayer);
	addAttribute(bumpTexLayer);
	addAttribute(diffuseReflection);
	addAttribute(mirrorStrength);
	addAttribute(transparency);
	addAttribute(translucency);
	addAttribute(transmitFilter);
	addAttribute(emit);
	addAttribute(fresnel);
	addAttribute(IOR);
	addAttribute(outShinyDiffuse);

//	attributeAffects(tString,outShinyDiffuse);
	attributeAffects(colorTexLayer,outShinyDiffuse);
	attributeAffects(mirrorTexLayer,outShinyDiffuse);
	attributeAffects(bumpTexLayer,outShinyDiffuse);
	attributeAffects(color,outShinyDiffuse);
	attributeAffects(mirrorColor,outShinyDiffuse);
	attributeAffects(diffuseReflection,outShinyDiffuse);
	attributeAffects(mirrorStrength,outShinyDiffuse);
	attributeAffects(transparency,outShinyDiffuse);
	attributeAffects(translucency,outShinyDiffuse);
	attributeAffects(transmitFilter,outShinyDiffuse);
	attributeAffects(emit,outShinyDiffuse);
	attributeAffects(fresnel,outShinyDiffuse);
	attributeAffects(IOR,outShinyDiffuse);

	

	return stat;
}

MStatus shinyDiffuseNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=outShinyDiffuse)&&(plug.parent() != outShinyDiffuse))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(color);
	const MFloatVector & iColor=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(outShinyDiffuse);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=iColor;
	outColorHandle.setClean();

	return stat;
}