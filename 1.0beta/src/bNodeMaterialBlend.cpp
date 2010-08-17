#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeMaterialBlend.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MGlobal.h>

const MTypeId blendNode::id(0x75307);

MObject blendNode::diffuseTexLayer;
MObject blendNode::materialA;
MObject blendNode::materialB;
MObject blendNode::blendValue;
MObject blendNode::outBlend;
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
void* blendNode::creator()
{
	return new blendNode;
}
MStatus blendNode::getAllMaterial(MStringArray &resultArray)
{
	MStatus stat;
	MString getCommand("ls -mat ");
	MGlobal::executeCommand(getCommand,resultArray);
	for (unsigned int index=0; index<resultArray.length(); index++)
	{
		if (resultArray[index] != "yaf*")
		{
			resultArray.remove(index);
		}
	}
	return stat;
}
MStatus blendNode::initialize()
{
	MStatus stat;
	//MFnEnumAttribute enumAttr;
	MFnNumericAttribute numAttr;
	MFnTypedAttribute tAttr;

	diffuseTexLayer=numAttr.createColor("DiffuseTextureLayer","dtl");
	MAKE_INPUT(numAttr);

	materialA=tAttr.create("MaterialA","ma",MFnData::kString);
	MAKE_INPUT(tAttr);

	materialB=tAttr.create("MaterialB","mb",MFnData::kString);
	MAKE_INPUT(tAttr);

	//use enum attribute got difficult....
	//MStringArray allYafMaterial;
	//getAllMaterial(allYafMaterial);
	//materialA=enumAttr.create("MaterialA","ma",0);
	//for (unsigned int index=0; index<allYafMaterial.length(); index++)
	//{
	//	enumAttr.addField(allYafMaterial[index], index);
	//}
	//enumAttr.setKeyable(true);
	//enumAttr.setStorable(true);

	//materialB=enumAttr.create("MaterialB","mb",0);
	//for (unsigned int index=0; index<allYafMaterial.length(); index++)
	//{
	//	enumAttr.addField(allYafMaterial[index], index);
	//}
	//enumAttr.setKeyable(true);
	//enumAttr.setStorable(true);

	blendValue=numAttr.create("BlendValue","bv",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	outBlend=numAttr.createColor("outColor","oc");
	MAKE_OUTPUT(numAttr);

	addAttribute(diffuseTexLayer);
	addAttribute(materialA);
	addAttribute(materialB);
	addAttribute(blendValue);
	addAttribute(outBlend);

	attributeAffects(diffuseTexLayer,outBlend);
	attributeAffects(materialA,outBlend);
	attributeAffects(materialB,outBlend);
	attributeAffects(blendValue,outBlend);

	return stat;
}
MStatus blendNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat;

	return stat;
}