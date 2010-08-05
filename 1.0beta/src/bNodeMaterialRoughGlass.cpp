#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bNodeMaterialRoughGlass.h"

#include<maya/MFnNumericAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId roughGlassNode::id(0x75311);

MObject roughGlassNode::exponent;
MObject roughGlassNode::alpha;
MObject roughGlassNode::absorbColor;
MObject roughGlassNode::absorbDistance;
MObject roughGlassNode::filterColor;
MObject roughGlassNode::mirrorColor;
MObject roughGlassNode::IOR;
MObject roughGlassNode::transmitFilter;
MObject roughGlassNode::dispersionPower;
MObject roughGlassNode::fakeShadows;

MObject roughGlassNode::outRoughGlass;

MStatus roughGlassNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=outRoughGlass)&&(plug.parent() != outRoughGlass))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(absorbColor);
	const MFloatVector & color=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(outRoughGlass);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();

	return stat;
}
void *roughGlassNode::creator()
{
	return new roughGlassNode;

}
MStatus roughGlassNode::initialize()
{
	MFnNumericAttribute numAttr;

	absorbColor=numAttr.createColor("AbsorbColor","gabco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	absorbDistance=numAttr.create("AbsorbDistance","gabdi",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(100.0f);

	filterColor=numAttr.createColor("FilterColor","rgfico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	mirrorColor=numAttr.createColor("MirrorColor","rgco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	IOR=numAttr.create("IOR","rgior",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(30.0f);

	transmitFilter=numAttr.create("TransmitFilter","rgtrfi",MFnNumericData::kDouble,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);

	dispersionPower=numAttr.create("DispersionPower","rgpo",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10000.0f);

	fakeShadows=numAttr.create("FakeShadows","rgfash",MFnNumericData::kBoolean);
	numAttr.setDefault(true);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	exponent=numAttr.create("Exponent","rgex",MFnNumericData::kFloat,500.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1.0f);
	numAttr.setMax(10000.0f);

	alpha=numAttr.create("Roughtness","rgr",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	outRoughGlass=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	numAttr.setReadable(true);
	numAttr.setWritable(false);

	addAttribute(alpha);
	addAttribute(absorbColor);
	addAttribute(absorbDistance);	
	addAttribute( filterColor );
	addAttribute(mirrorColor);	
	addAttribute(IOR);
	addAttribute(transmitFilter);
	addAttribute(dispersionPower);
	addAttribute(fakeShadows);
	addAttribute(exponent);
	addAttribute(outRoughGlass);

	attributeAffects(alpha,outRoughGlass);
	attributeAffects(absorbColor,outRoughGlass);
	attributeAffects(absorbDistance,outRoughGlass);
	attributeAffects(filterColor,outRoughGlass);
	attributeAffects(mirrorColor,outRoughGlass);
	attributeAffects(IOR,outRoughGlass);
	attributeAffects(transmitFilter,outRoughGlass);
	attributeAffects(dispersionPower,outRoughGlass);
	attributeAffects(fakeShadows,outRoughGlass);
	attributeAffects(exponent,outRoughGlass);



	return MStatus::kSuccess;

}