#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeTextureImage.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include<maya/MFloatVector.h>
#include<maya/MDataHandle.h>
#include<maya/MPlug.h>
#include<maya/MDataBlock.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatVector.h>

const MTypeId imageTexNode::id(0x75315);

MObject imageTexNode::imageOriginColor;
MObject imageTexNode::imageFile;
MObject imageTexNode::imageRepeatX;
MObject imageTexNode::imagerepeatY;
MObject imageTexNode::imageUseAlpha;
MObject imageTexNode::imageCalcAlpha;
MObject imageTexNode::imageNormalMap;
MObject imageTexNode::imageMinX;
MObject imageTexNode::imageMinY;
MObject imageTexNode::imageMaxX;
MObject imageTexNode::imageMaxY;
MObject imageTexNode::imageUV;
MObject imageTexNode::imageUVFilterSize;
MObject imageTexNode::imageOutput;

void * imageTexNode::creator()
{
	return new imageTexNode;
}
MStatus imageTexNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;
	MFnEnumAttribute enumAttr;

	imageOriginColor=numAttr.createColor("OriginalColor","orc");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	imageFile=numAttr.createColor("ImageInput","ii");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	imageRepeatX=numAttr.create("RepeatX","rx",MFnNumericData::kInt,1);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1);
	numAttr.setMax(512);

	imagerepeatY=numAttr.create("RepeatY","ry",MFnNumericData::kInt,1);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1);
	numAttr.setMax(512);

	imageUseAlpha=numAttr.create("UseAlpha","ua",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	imageCalcAlpha=numAttr.create("CalculateAlpha","ca",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	imageNormalMap=numAttr.create("NormalMap","nm",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	imageMinX=numAttr.create("MinX","minx",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMinY=numAttr.create("MinY","miny",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMaxX=numAttr.create("MaxX","maxx",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	imageMaxY=numAttr.create("MaxY","maxy",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(-10.0);
	numAttr.setMax(10.0);

	MObject u=numAttr.create("uCoord","u",MFnNumericData::kFloat);
	MObject v=numAttr.create("vCoord","v",MFnNumericData::kFloat);
	imageUV=numAttr.create("uvCoord","uv",u,v);
	numAttr.setStorable(true);
	numAttr.setHidden(true);

	MObject filterX=numAttr.create("uvFilterSizeX", "fsx", MFnNumericData::kFloat);
	MObject filterY=numAttr.create( "uvFilterSizeY", "fsy", MFnNumericData::kFloat);
	imageUVFilterSize=numAttr.create("uvFilterSize", "fs", filterX, filterY);
	numAttr.setStorable(true);
	numAttr.setHidden(true);

	imageOutput=numAttr.createColor("outColor","oc");
	numAttr.setHidden(true);
	numAttr.setReadable(true);
	numAttr.setWritable(false);

	addAttribute(imageOriginColor);
	addAttribute(imageFile);
	addAttribute(imageRepeatX);
	addAttribute(imagerepeatY);
	addAttribute(imageUseAlpha);
	addAttribute(imageCalcAlpha);
	addAttribute(imageNormalMap);
	addAttribute(imageMinX);
	addAttribute(imageMinY);
	addAttribute(imageMaxX);
	addAttribute(imageMaxY);
	addAttribute(imageUV);
	addAttribute(imageUVFilterSize);
	addAttribute(imageOutput);

	attributeAffects(imageOriginColor,imageOutput);
	attributeAffects(imageFile,imageOutput);
	attributeAffects(imageRepeatX,imageOutput);
	attributeAffects(imagerepeatY,imageOutput);
	attributeAffects(imageUseAlpha,imageOutput);
	attributeAffects(imageCalcAlpha,imageOutput);
	attributeAffects(imageNormalMap,imageOutput);
	attributeAffects(imageMinX,imageOutput);
	attributeAffects(imageMinY,imageOutput);
	attributeAffects(imageMaxX,imageOutput);
	attributeAffects(imageMaxY,imageOutput);
	attributeAffects(imageUV,imageOutput);
	attributeAffects(imageUVFilterSize,imageOutput);

	return stat;
}
MStatus imageTexNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat=MStatus::kSuccess;
	if ((plug !=imageOutput)&&(plug.parent() != imageOutput))
	{
		return MStatus::kUnknownParameter;
	}

	MDataHandle indexColor=data.inputValue(imageOriginColor);
	const MFloatVector & color=indexColor.asFloatVector();

	MDataHandle outColorHandle=data.outputValue(imageOutput);
	MFloatVector & outColor=outColorHandle.asFloatVector();

	outColor=color;
	outColorHandle.setClean();

	return stat;

}