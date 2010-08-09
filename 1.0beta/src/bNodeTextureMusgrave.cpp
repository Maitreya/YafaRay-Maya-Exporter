#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureMusgrave.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId musgraveTexNode::id(0x75321);

MObject musgraveTexNode::musgraveType;
MObject musgraveTexNode::musgraveH;
MObject musgraveTexNode::musgraveLacunarity;
MObject musgraveTexNode::musgraveOctaves;
MObject musgraveTexNode::musgraveSize;
MObject musgraveTexNode::musgraveIntensity;

MObject musgraveTexNode::NoiseType;
MObject musgraveTexNode::mappingMethod;
MObject musgraveTexNode::texCo;
MObject musgraveTexNode::UV;
MObject musgraveTexNode::UVFilterSize;
MObject musgraveTexNode::Output;

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

void* musgraveTexNode::creator()
{
	return new musgraveTexNode;
}
MStatus musgraveTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	musgraveType=enumAttr.create("MusgraveType","mut",0);
	enumAttr.addField("fbm",0);
	enumAttr.addField("multifractal",1);
	enumAttr.addField("ridgedmf",2);
	enumAttr.addField("hybridmf",3);
	enumAttr.addField("heteroterrain",4);
	MAKE_INPUT(enumAttr);

	musgraveH=numAttr.create("H","h",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.0);
	numAttr.setMax(2.0);
	MAKE_INPUT(numAttr);

	musgraveLacunarity=numAttr.create("Lacunarity","lac",MFnNumericData::kFloat,2.0);
	numAttr.setMin(0.0);
	numAttr.setMax(6.0);
	MAKE_INPUT(numAttr);

	musgraveOctaves=numAttr.create("Octaves","oct",MFnNumericData::kFloat,2.0);
	numAttr.setMin(0.0);
	numAttr.setMax(8.0);
	MAKE_INPUT(numAttr);

	musgraveSize=numAttr.create("MusgraveSize","mus",MFnNumericData::kFloat,0.0);
	numAttr.setMin(0.0);
	numAttr.setMax(2.0);
	MAKE_INPUT(numAttr);

	musgraveIntensity=numAttr.create("Intensity","intn",MFnNumericData::kFloat,1.0);
	numAttr.setMin(0.01);
	numAttr.setMax(10.0);
	MAKE_INPUT(numAttr);


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


	addAttribute(musgraveType);
	addAttribute(musgraveH);
	addAttribute(musgraveLacunarity);
	addAttribute(musgraveOctaves);
	addAttribute(musgraveSize);
	addAttribute(musgraveIntensity);
	addAttribute(NoiseType);
	addAttribute(mappingMethod);
	addAttribute(texCo);
	addAttribute(UV);
	addAttribute(UVFilterSize);
	addAttribute(Output);


	attributeAffects(musgraveType,Output);
	attributeAffects(musgraveH,Output);
	attributeAffects(musgraveLacunarity,Output);
	attributeAffects(musgraveOctaves,Output);
	attributeAffects(musgraveSize,Output);
	attributeAffects(musgraveIntensity,Output);
	attributeAffects(NoiseType,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);
	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;
}
MStatus musgraveTexNode::compute(const MPlug &plug, MDataBlock &data)
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