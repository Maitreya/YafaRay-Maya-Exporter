#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bNodeMaterialGlass.h"

#include<maya/MFnNumericAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId glassNode::id(0x75300);

MObject glassNode::absorbColor;
MObject glassNode::absorbDistance;
MObject glassNode::filterColor;
MObject glassNode::mirrorColor;
MObject glassNode::IOR;
MObject glassNode::transmitFilter;
MObject glassNode::dispersionPower;
MObject glassNode::fakeShadows;

MObject glassNode::outGlass;

MStatus glassNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=outGlass)&&(plug.parent() != outGlass))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(absorbColor);
	const MFloatVector & color=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(outGlass);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();

	return stat;
}
void *glassNode::creator()
{
	return new glassNode;

}
MStatus glassNode::initialize()
{
	MFnNumericAttribute numAttr;

	absorbColor=numAttr.createColor("AbsorbColor","gabco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	absorbDistance=numAttr.create("AbsorbDistance","gabdi",MFnNumericData::kFloat);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(100.0f);
	numAttr.setDefault(1.0f);

	filterColor=numAttr.createColor("FilterColor","gfico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	mirrorColor=numAttr.createColor("MirrorColor","gmico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	IOR=numAttr.create("IOR","gior",MFnNumericData::kFloat);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(30.0f);
	numAttr.setDefault(1.0f);

	transmitFilter=numAttr.create("TransmitFilter","gtrfi",MFnNumericData::kDouble);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	numAttr.setDefault(1.0);

	dispersionPower=numAttr.create("DispersionPower","gdipo",MFnNumericData::kFloat);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10000.0f);
	numAttr.setDefault(0.0f);

	fakeShadows=numAttr.create("FakeShadows","gfash",MFnNumericData::kBoolean);
	numAttr.setDefault(true);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	

	outGlass=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	numAttr.setReadable(true);
	numAttr.setWritable(false);

	addAttribute(absorbColor);
	addAttribute(absorbDistance);	
	addAttribute( filterColor );
	addAttribute(mirrorColor);	
	addAttribute(IOR);
	addAttribute(transmitFilter);
	addAttribute(dispersionPower);
	addAttribute(fakeShadows);
	addAttribute(outGlass);

	attributeAffects(absorbColor,outGlass);
	attributeAffects(absorbDistance,outGlass);
	attributeAffects(filterColor,outGlass);
	attributeAffects(mirrorColor,outGlass);
	attributeAffects(IOR,outGlass);
	attributeAffects(transmitFilter,outGlass);
	attributeAffects(dispersionPower,outGlass);
	attributeAffects(fakeShadows,outGlass);



	return MStatus::kSuccess;

}