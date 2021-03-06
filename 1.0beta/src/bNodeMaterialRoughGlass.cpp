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

MObject roughGlassNode::mirrorTexLayer;
MObject roughGlassNode::filterTexLayer;
MObject roughGlassNode::bumpTexLayer;

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

	mirrorTexLayer=numAttr.createColor("MirrorTextureLayer","mtl"); 	MAKE_INPUT(numAttr);
	filterTexLayer=numAttr.createColor("FilterTextureLayer","ftl"); 	MAKE_INPUT(numAttr);
	bumpTexLayer=numAttr.createColor("BumpTextureLayer","btl"); 	MAKE_INPUT(numAttr);

	absorbColor=numAttr.createColor("AbsorbColor","gabco");
	MAKE_INPUT(numAttr);

	absorbDistance=numAttr.create("AbsorbDistance","gabdi",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(100.0f);

	filterColor=numAttr.createColor("FilterColor","rgfico");
	MAKE_INPUT(numAttr);

	mirrorColor=numAttr.createColor("MirrorColor","rgco");
	MAKE_INPUT(numAttr);

	IOR=numAttr.create("IOR","rgior",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(30.0f);

	transmitFilter=numAttr.create("TransmitFilter","rgtrfi",MFnNumericData::kDouble,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);

	dispersionPower=numAttr.create("DispersionPower","rgpo",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(10000.0f);

	fakeShadows=numAttr.create("FakeShadows","rgfash",MFnNumericData::kBoolean,true);
	MAKE_INPUT(numAttr);

	exponent=numAttr.create("Exponent","rgex",MFnNumericData::kFloat,500.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(1.0f);
	numAttr.setMax(10000.0f);

	alpha=numAttr.create("Roughness","rgr",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(1.0f);

	outRoughGlass=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	MAKE_OUTPUT(numAttr);

	addAttribute(alpha);
	addAttribute(absorbColor);
	addAttribute(absorbDistance);	
	addAttribute( filterColor );
	addAttribute(filterTexLayer);
	addAttribute(mirrorColor);
	addAttribute(mirrorTexLayer);
	addAttribute(bumpTexLayer);
	addAttribute(IOR);
	addAttribute(transmitFilter);
	addAttribute(dispersionPower);
	addAttribute(fakeShadows);
	addAttribute(exponent);
	addAttribute(outRoughGlass);

	attributeAffects(mirrorTexLayer,outRoughGlass);
	attributeAffects(filterTexLayer,outRoughGlass);
	attributeAffects(bumpTexLayer,outRoughGlass);
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