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

MObject voronoiTexNode::NoiseType;
MObject voronoiTexNode::mappingMethod;
MObject voronoiTexNode::texCo;
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

	voronoiSize=numAttr.create("VoronoiSize","vosi",MFnNumericData::kFloat,0.0);
	numAttr.setMin(0.0);
	numAttr.setMax(2.0);
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

	NoiseType=enumAttr.create("NoiseType","noty",0);
	enumAttr.addField("newperlin",0);
	enumAttr.addField("stdperlin",1);
	enumAttr.addField("voronoi_f1",2);
	enumAttr.addField("voronoi_f2",3);
	enumAttr.addField("voronoi_f3",4);
	enumAttr.addField("voronoi_f4",5);
	enumAttr.addField("voronoi_f2fl",6);
	enumAttr.addField("voronoi_crackle",7);
	enumAttr.addField("cellnoise",8);
	MAKE_INPUT(enumAttr);

	mappingMethod=enumAttr.create("MappingMethod","mame",0);
	enumAttr.addField("uv",0);
	enumAttr.addField("orco",1);
	enumAttr.addField("global",2);
	enumAttr.addField("window",3);
	MAKE_INPUT(enumAttr);

	texCo=enumAttr.create("TextureCoordinate","teco",0);
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
	addAttribute(NoiseType);
	addAttribute(mappingMethod);
	addAttribute(texCo);
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
	attributeAffects(NoiseType,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;

}
MStatus voronoiTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=Output)&&(plug.parent() !=Output))
	{
		return MStatus::kUnknownParameter;
	}

	const MFloatVector color(0.0,0.0,0.0);

	MDataHandle outColorHandle=data.outputValue(Output);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();
	return stat;
}