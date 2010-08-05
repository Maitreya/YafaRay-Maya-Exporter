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
MObject shinyDiffuseNode::mirrorColor;
MObject shinyDiffuseNode::diffuseReflection;
MObject shinyDiffuseNode::mirrorStrength;
MObject shinyDiffuseNode::transparency;
MObject shinyDiffuseNode::translucency;
MObject shinyDiffuseNode::transmitFilter;
MObject shinyDiffuseNode::emit;
MObject shinyDiffuseNode::fresnel;
MObject shinyDiffuseNode::IOR;
MObject shinyDiffuseNode::outShinyDiffuse;

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

	color=numAttr.createColor("Color","dco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	mirrorColor=numAttr.createColor("MirrorColor","dmico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	diffuseReflection=numAttr.create("DiffuseReflection","ddire",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	mirrorStrength=numAttr.create("MirrorStrength","dmist",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	transparency=numAttr.create("Transparency","dtr",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	translucency=numAttr.create("Translucency","dtra",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	transmitFilter=numAttr.create("TransmitFilter","dtrfi",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	emit=numAttr.create("Emit","de",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1000.0f);

	fresnel=numAttr.create("Fresnel","dfr",MFnNumericData::kBoolean,false);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	IOR=numAttr.create("IOR","dior",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1.0f);
	numAttr.setMax(30.0f);

	outShinyDiffuse=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	numAttr.setReadable(true);
	numAttr.setWritable(false);

//	addAttribute(tString);
	addAttribute(color);
	addAttribute(mirrorColor);
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