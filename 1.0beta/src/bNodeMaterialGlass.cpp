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

MObject glassNode::mirrorTexLayer;
MObject glassNode::filterTexLayer;
MObject glassNode::bumpTexLayer;

MObject glassNode::absorbColor;
MObject glassNode::absorbDistance;
MObject glassNode::filterColor;
MObject glassNode::mirrorColor;
MObject glassNode::IOR;
MObject glassNode::transmitFilter;
MObject glassNode::dispersionPower;
MObject glassNode::fakeShadows;

MObject glassNode::outGlass;

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

	mirrorTexLayer=numAttr.createColor("MirrorTextureLayer","mtl"); 	MAKE_INPUT(numAttr);
	filterTexLayer=numAttr.createColor("FilterTextureLayer","ftl"); 	MAKE_INPUT(numAttr);
	bumpTexLayer=numAttr.createColor("BumpTextureLayer","btl"); 	MAKE_INPUT(numAttr);

	absorbColor=numAttr.createColor("AbsorbColor","gabco");
	MAKE_INPUT(numAttr);

	absorbDistance=numAttr.create("AbsorbDistance","gabdi",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(100.0f);

	filterColor=numAttr.createColor("FilterColor","gfico");
	MAKE_INPUT(numAttr);

	mirrorColor=numAttr.createColor("MirrorColor","gmico");
	MAKE_INPUT(numAttr);

	IOR=numAttr.create("IOR","gior",MFnNumericData::kFloat,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(30.0f);

	transmitFilter=numAttr.create("TransmitFilter","gtrfi",MFnNumericData::kDouble,1.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);

	dispersionPower=numAttr.create("DispersionPower","gdipo",MFnNumericData::kFloat,0.0);
	MAKE_INPUT(numAttr);
	numAttr.setMin(0.0f);
	numAttr.setMax(10000.0f);

	fakeShadows=numAttr.create("FakeShadows","gfash",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);
	

	outGlass=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	MAKE_OUTPUT(numAttr);

	
	
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
	addAttribute(outGlass);

	attributeAffects(mirrorTexLayer,outGlass);
	attributeAffects(filterTexLayer,outGlass);
	attributeAffects(bumpTexLayer,outGlass);
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