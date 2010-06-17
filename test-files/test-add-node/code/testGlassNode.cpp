#include"testGlassNode.h"
#include<maya/MFnNumericAttribute.h>

const MTypeId testGlassNode::id(0x75300);

MObject testGlassNode::absorbColor;
MObject testGlassNode::absorbDistance;
MObject testGlassNode::filterColor;
MObject testGlassNode::mirrorColor;
MObject testGlassNode::IOR;
MObject testGlassNode::transmitFilter;
MObject testGlassNode::dispersionPower;
MObject testGlassNode::fakeShadows;

MObject testGlassNode::outGlass;

MStatus testGlassNode::compute(const MPlug &plug, MDataBlock &data)
{
	return MStatus::kSuccess;

}
void *testGlassNode::creator()
{
	return new testGlassNode;

}
MStatus testGlassNode::initialize()
{
	MFnNumericAttribute numAttr;

	absorbColor=numAttr.createColor("AbsorbColor","abco");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	absorbDistance=numAttr.create("AbsorbDistance","abdi",MFnNumericData::kFloat);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(100.0f);
	numAttr.setDefault(1.0f);

	filterColor=numAttr.createColor("FilterColor","fico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	mirrorColor=numAttr.createColor("MirrorColor","mico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	IOR=numAttr.create("IOR","ior",MFnNumericData::kFloat);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(30.0f);
	numAttr.setDefault(1.0f);

	transmitFilter=numAttr.create("TransmitFilter","trfi",MFnNumericData::kDouble);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	numAttr.setDefault(1.0);

	dispersionPower=numAttr.create("DispersionPower","dipo",MFnNumericData::kFloat);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0f);
	numAttr.setMax(10000.0f);
	numAttr.setDefault(0.0f);

	fakeShadows=numAttr.create("FakeShadows","fash",MFnNumericData::kBoolean);
	numAttr.setDefault(true);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	

	outGlass=numAttr.create("OutGlass","ougl",MFnNumericData::kBoolean);
	numAttr.setDefault(true);
	numAttr.setHidden(true);


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

