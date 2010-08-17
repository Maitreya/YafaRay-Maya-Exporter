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

//texture layer settings
MObject musgraveTexNode::layerMix;
MObject musgraveTexNode::textureColor;
MObject musgraveTexNode::texColorFact;
MObject musgraveTexNode::defVal;
MObject musgraveTexNode::valFact;
MObject musgraveTexNode::doColor;
MObject musgraveTexNode::negative;
MObject musgraveTexNode::noRGB;
MObject musgraveTexNode::stencil;

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

	musgraveSize=numAttr.create("MusgraveSize","mus",MFnNumericData::kFloat,4.0);
	numAttr.setMin(0.0);
	numAttr.setMax(32.0);
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

	texCo=enumAttr.create("TextureCoordinate","texco",0);
	enumAttr.addField("plain",0);
	enumAttr.addField("cube",1);
	enumAttr.addField("tube",2);
	enumAttr.addField("sphere",3);
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


	attributeAffects(musgraveType,Output);
	attributeAffects(musgraveH,Output);
	attributeAffects(musgraveLacunarity,Output);
	attributeAffects(musgraveOctaves,Output);
	attributeAffects(musgraveSize,Output);
	attributeAffects(musgraveIntensity,Output);
	attributeAffects(NoiseType,Output);
	attributeAffects(mappingMethod,Output);
	attributeAffects(texCo,Output);

	attributeAffects(layerMix,Output);
	attributeAffects(textureColor,Output);
	attributeAffects(texColorFact,Output);
	attributeAffects(defVal,Output);
	attributeAffects(valFact,Output);
	attributeAffects(doColor,Output);
	attributeAffects(negative,Output);
	attributeAffects(noRGB,Output);
	attributeAffects(stencil,Output);

	attributeAffects(UV,Output);
	attributeAffects(UVFilterSize,Output);

	return stat;
}
MStatus musgraveTexNode::compute(const MPlug &plug, MDataBlock &data)
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