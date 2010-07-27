#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeMaterialCoatedGlossy.h"

#include<maya/MFnNumericAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId coatedGlossyNode::id(0x75305);

MObject coatedGlossyNode::diffuseColor;
MObject coatedGlossyNode::diffuseReflect;
MObject coatedGlossyNode::glossyColor;
MObject coatedGlossyNode::glossyReflect;
MObject coatedGlossyNode::exponent;
MObject coatedGlossyNode::asDiffuse;
MObject coatedGlossyNode::anisotropic;
MObject coatedGlossyNode::expHorizontal;
MObject coatedGlossyNode::expVertical;
MObject coatedGlossyNode::IOR;

MObject coatedGlossyNode::outCoatedGlossy;

void *coatedGlossyNode::creator()
{
	return new coatedGlossyNode;
}
MStatus coatedGlossyNode::initialize()
{
	MStatus stat=MStatus::kSuccess;
	MFnNumericAttribute numAttr;

	 diffuseColor=numAttr.createColor("DiffuseColor","cdico");
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setDefault(1.0,1.0,1.0);

	 diffuseReflect=numAttr.create("DiffuseReflection","cdire",MFnNumericData::kFloat,1.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0f);
	 numAttr.setMax(1.0f);

	 glossyColor=numAttr.createColor("GlossyColor","cglco");
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setDefault(1.0,1.0,1.0);

	 glossyReflect=numAttr.create("GlossyReflection","cglre",MFnNumericData::kFloat,0.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(0.0f);
	 numAttr.setMax(1.0f);

	 exponent=numAttr.create("Exponent","cex",MFnNumericData::kFloat,500.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1.0f);
	 numAttr.setMax(5000.0f);

	 asDiffuse=numAttr.create("AsDiffuse","casdi",MFnNumericData::kBoolean,false);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 anisotropic=numAttr.create("Anisotropic","can",MFnNumericData::kBoolean,false);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);

	 expHorizontal=numAttr.create("ExponentHorizontal","cexho",MFnNumericData::kFloat,50.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1.0f);
	 numAttr.setMax(10000.0f);

	 expVertical=numAttr.create("ExponentVertical","cexve",MFnNumericData::kFloat,50.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1.0f);
	 numAttr.setMax(10000.0f);

	 IOR=numAttr.create("IOR","cior",MFnNumericData::kFloat,1.0);
	 numAttr.setKeyable(true);
	 numAttr.setStorable(true);
	 numAttr.setMin(1.0f);
	 numAttr.setMax(30.0f);

	 outCoatedGlossy=numAttr.createColor("outColor","oc");
	 numAttr.setHidden(true);
	 numAttr.setReadable(true);
	 numAttr.setWritable(false);

	 addAttribute(diffuseColor);
	 addAttribute(diffuseReflect);
	 addAttribute(glossyColor);
	 addAttribute(glossyReflect);
	 addAttribute(exponent);
	 addAttribute(asDiffuse);
	 addAttribute(anisotropic);
	 addAttribute(expHorizontal);
	 addAttribute(expVertical);
	 addAttribute(IOR);
	 addAttribute(outCoatedGlossy);

	 attributeAffects(diffuseColor,outCoatedGlossy);
	 attributeAffects(diffuseReflect,outCoatedGlossy);
	 attributeAffects(glossyColor,outCoatedGlossy);
	 attributeAffects(glossyReflect,outCoatedGlossy);
	 attributeAffects(exponent,outCoatedGlossy);
	 attributeAffects(asDiffuse,outCoatedGlossy);
	 attributeAffects(anisotropic,outCoatedGlossy);
	 attributeAffects(expHorizontal,outCoatedGlossy);
	 attributeAffects(expVertical,outCoatedGlossy);
	 attributeAffects(IOR,outCoatedGlossy);

	return stat;
}

MStatus coatedGlossyNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=outCoatedGlossy)&&(plug.parent() != outCoatedGlossy))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(glossyColor);
	const MFloatVector & color=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(outCoatedGlossy);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();

	return stat;
}