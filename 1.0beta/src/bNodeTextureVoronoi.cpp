#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureVoronoi.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId voronoiTexNode::id(0x75320);

MObject voronoiTexNode::voronoiColorType;
MObject voronoiTexNode::voronoiWeight1;
MObject voronoiTexNode::voronoiWeight2;
MObject voronoiTexNode::voronoiWeight3;
MObject voronoiTexNode::voronoiWeight4;
MObject voronoiTexNode::voronoiExponent;
MObject voronoiTexNode::voronoiIntensity;
MObject voronoiTexNode::voronoiSize;
MObject voronoiTexNode::voronoiDistanceMetric;

//MObject voronoiTexNode::NoiseType;

//mapping method
MObject voronoiTexNode::mappingMethod;
MObject voronoiTexNode::texCo;

//texture layer settings
MObject voronoiTexNode::layerMix;
MObject voronoiTexNode::textureColor;
MObject voronoiTexNode::texColorFact;
MObject voronoiTexNode::defVal;
MObject voronoiTexNode::valFact;
MObject voronoiTexNode::doColor;
MObject voronoiTexNode::negative;
MObject voronoiTexNode::noRGB;
MObject voronoiTexNode::stencil;

MObject voronoiTexNode::UV;
MObject voronoiTexNode::UVFilterSize;
MObject voronoiTexNode::Output;

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

void* voronoiTexNode::creator()
{
	return new voronoiTexNode;
}
MStatus voronoiTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	voronoiColorType=enumAttr.create("ColorType","coty",0);
	enumAttr.addField("int",0);
	enumAttr.addField("color1",1);
	enumAttr.addField("color2",2);
	enumAttr.addField("color3",3);
	MAKE_INPUT(enumAttr);

	voronoiWeight1=numAttr.create("Weight1","w1",MFnNumericData::kFloat,1.0);
	numAttr.setMin(-2.0);
	numAttr.setMax(2.0);
	MAKE_INPUT(numAttr);

	voronoiWeight2=numAttr.create("Weight2","w2",MFnNumericData::kFloat,0.0);
	numAttr.setMin(-2.0);
	numAttr.setMax(2.0);
	MAKE_INPUT(numAttr);

	voronoiWeight3=numAttr.create("Weight3","w3",MFnNumericData::kFloat,0.0);
	numAttr.setMin(-2.0);
	numAttr.setMax(2.0);
	MAKE_INPUT(numAttr);

	voronoiWeight4=numAttr.create("Weight4","w4",MFnNumericData::kFloat,0.0);
	numAttr.setMin(-2.0);
	numAttr.setMax(2.0);
	MAKE_INPUT(numAttr);

	voronoiExponent=numAttr.create("Exponent","exp",MFnNumericData::kFloat,2.5);
	numAttr.setMin(0.01);
	numAttr.setMax(10.0);
	MAKE_INPUT(numAttr);

	voronoiIntensity=numAttr.create("Intensity","intn",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.01);
	numAttr.setMax(10.0);
	MAKE_INPUT(numAttr);

	voronoiSize=numAttr.create("VoronoiSize","vosi",MFnNumericData::kFloat,4);
	numAttr.setMin(0.0);
	numAttr.setMax(32.0);
	MAKE_INPUT(numAttr);

	voronoiDistanceMetric=enumAttr.create("DistanceMetric","dime",0);
	enumAttr.addField("actual",0);
	enumAttr.addField("squared",1);
	enumAttr.addField("manhattan",2);
	enumAttr.addField("chebychev",3);
	enumAttr.addField("minkovsky_half",4);
	enumAttr.addField("minkovsky_four",5);
	enumAttr.addField("minkovsky",6);
	MAKE_INPUT(enumAttr);

	//*******************************layer texture attribute*********************************//
	layerMix=enumAttr.create("MixMethod","mm1",0);
	enumAttr.addField("mix",0);
	enumAttr.addField("add",1);
	enumAttr.addField("multiply",2);
	enumAttr.addField("subtract",3);
	enumAttr.addField("screen",4);
	enumAttr.addField("divide",5);
	enumAttr.addField("difference",6);
	enumAttr.addField("darken",7);
	enumAttr.addField("lighten",8);
	MAKE_INPUT(enumAttr);

	textureColor=numAttr.createColor("TextureColor","teco");
	numAttr.setDefault(1.0,0.0,1.0);
	MAKE_INPUT(numAttr);

	texColorFact=numAttr.create("TextureColorWeight","tcw",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	MAKE_INPUT(numAttr);

	defVal=numAttr.create("DefValue","dev",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	MAKE_INPUT(numAttr);

	valFact=numAttr.create("ValueWeight","vaw",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0);
	MAKE_INPUT(numAttr);

	doColor=numAttr.create("DoColor","doco",MFnNumericData::kBoolean,true);
	MAKE_INPUT(numAttr);

	negative=numAttr.create("Negative","nega",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

	noRGB=numAttr.create("NoRGB","nr",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);

	stencil=numAttr.create("Stencil","sten",MFnNumericData::kBoolean,false);
	MAKE_INPUT(numAttr);
	//*******************************layer texture attribute end*********************************//

	mappingMethod=enumAttr.create("MappingMethod","mame",0);
	enumAttr.addField("uv",0);
	enumAttr.addField("orco",1);
	enumAttr.addField("global",2);
	enumAttr.addField("window",3);
	MAKE_INPUT(enumAttr);

	texCo=enumAttr.create("TextureCoordinate","texco",0);
	enumAttr.addField("plain",0);
	enumAttr.addField("cube",1);
	enumAttr.addField("tube",2);
	enumAttr.addField("sphere",3);
	MAKE_INPUT(enumAttr);

	MObject u=numAttr.create("uCoord","u",MFnNumericData::kFloat);
	MObject v=numAttr.create("vCoord","v",MFnNumericData::kFloat);
	UV=numAttr.create("uvCoord","uv",u,v);
	MAKE_INPUT(numAttr);

	MObject filterX=numAttr.create("uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	MObject filterY=numAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	UVFilterSize=numAttr.create("uvFilterSize", "fs", filterX, filterY);
	MAKE_INPUT(numAttr);

	Output=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	MAKE_OUTPUT(numAttr);

	addAttribute(voronoiColorType);
	addAttribute(voronoiWeight1);
	addAttribute(voronoiWeight2);
	addAttribute(voronoiWeight3);
	addAttribute(voronoiWeight4);
	addAttribute(voronoiExponent);
	addAttribute(voronoiIntensity);
	addAttribute(voronoiSize);
	addAttribute(voronoiDistanceMetric);
//	addAttribute(NoiseType);
	addAttribute(mappingMethod);
	addAttribute(texCo);

	addAttribute(layerMix);
	addAttribute(textureColor);
	addAttribute(texColorFact);
	addAttribute(defVal);
	addAttribute(valFact);
	addAttribute(doColor);
	addAttribute(negative);
	addAttribute(noRGB);
	addAttribute(stencil);

	addAttribute(UV);
	addAttribute(UVFilterSize);
	addAttribute(Output);


	attributeAffects(voronoiColorType,Output);
	attributeAffects(voronoiWeight1,Output);
	attributeAffects(voronoiWeight2,Output);
	attributeAffects(voronoiWeight3,Output);
	attributeAffects(voronoiWeight4,Output);
	attributeAffects(voronoiExponent,Output);
	attributeAffects(voronoiIntensity,Output);
	attributeAffects(voronoiSize,Output);
	attributeAffects(voronoiDistanceMetric,Output);
//	attributeAffects(NoiseType,Output);

	attributeAffects(layerMix,Output);
	attributeAffects(textureColor,Output);
	attributeAffects(texColorFact,Output);
	attributeAffects(defVal,Output);
	attributeAffects(valFact,Output);
	attributeAffects(doColor,Output);
	attributeAffects(negative,Output);
	attributeAffects(noRGB,Output);
	attributeAffects(stencil,Output);

	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;

}
MStatus voronoiTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=Output)&&(plug.parent() != Output))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(textureColor);
	const MFloatVector & iColor=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(Output);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=iColor;
	outColorHandle.setClean();

	return stat;
}