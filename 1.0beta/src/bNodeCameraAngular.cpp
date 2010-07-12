#include"bNodeCameraAngular.h"
#include<maya/MFnNumericAttribute.h>

const MTypeId angularNode::id(0x75303);

	MObject angularNode::angularCircular;
    MObject angularNode::angularMirrored;
	MObject angularNode::angularMaxAngle;
	MObject angularNode::angularAngle;
	MObject angularNode::angularOutAngular;

void * angularNode::creator()
{
	return new angularNode;
}
MStatus angularNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	return stat;
}
MStatus angularNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;

	angularCircular=numAttr.create("Circular","acir",MFnNumericData::kBoolean,1);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	angularMirrored=numAttr.create("Mirrored","amir",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	angularMaxAngle=numAttr.create("MaxAngle","amaan",MFnNumericData::kFloat,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(180.0);

	angularAngle=numAttr.create("Angle","aang",MFnNumericData::kFloat,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(180.0);

	angularOutAngular=numAttr.create("OutAngular","aouan",MFnNumericData::kBoolean,1);
	numAttr.setHidden(true);


	addAttribute(angularCircular);
    addAttribute(angularMirrored);
	addAttribute(angularMaxAngle);
	addAttribute(angularAngle);
	addAttribute(angularOutAngular);

	attributeAffects(angularCircular,angularOutAngular);
    attributeAffects(angularMirrored,angularOutAngular);
	attributeAffects(angularMaxAngle,angularOutAngular);
	attributeAffects(angularAngle,angularOutAngular);


	return stat=MStatus::kSuccess;
}