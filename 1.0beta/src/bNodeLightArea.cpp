#define NOMINMAX
#define _USE_MATH_DEFINES 1

#include "bNodeLightArea.h"

#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MColor.h>

const MTypeId areaLightNode::id(0x75308);
MObject areaLightNode::areaLightColor;
MObject areaLightNode::areaPower;
MObject areaLightNode::areaSamples;
MObject areaLightNode::areaMakeGeo;

void * areaLightNode::creator()
{
	return new areaLightNode;
}
MStatus areaLightNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;

	areaLightColor=numAttr.createColor("LightColor","lico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.5,0.5,0.5);

	areaPower=numAttr.create("Power","ap",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	areaSamples=numAttr.create("Samples","as",MFnNumericData::kInt,1);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0);
	numAttr.setMax(512); 

	areaMakeGeo=numAttr.create("MakeGeometry","mg",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	addAttribute(areaLightColor);
	addAttribute(areaPower);
	addAttribute(areaSamples);
	addAttribute(areaMakeGeo);

	return stat;
}
void areaLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{

	MColor drawColor;

	drawColor.set(MColor::kRGB,1,1,0,0.5);

	if (status==M3dView::kDormant)
	{
		drawColor.set(MColor::kRGB,1,0,0,0.5);
	}

	view.setDrawColor(drawColor);
	view.beginGL();
	glBegin(GL_LINES);
	    //these are useful
	    glVertex3f(-1.0f,-1.0f,0.0f);
		glVertex3f(-1.0f,1.0f,0.0f);

		glVertex3f(-1.0f,1.0f,0.0f);
		glVertex3f(1.0f,1.0f,0.0f);

		glVertex3f(1.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,0.0f);

		glVertex3f(1.0f,-1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);

		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,-3.0f);

		glVertex3f(0.0f,0.0f,-3.0f);
		glVertex3f(0.0f,-0.1f,-2.8f);

		glVertex3f(0.0f,0.0f,-3.0f);
		glVertex3f(0.0f,0.1f,-2.8f);

		//these just for fun......
		glVertex3f(-0.3f,0.5f,0.0f);
		glVertex3f(-0.85f,-0.3f,0.0f);

		glVertex3f(0.3f,0.5f,0.0f);
		glVertex3f(0.85f,-0.3f,0.0f);

		glVertex3f(-0.4f,-1.0f,0.0f);
		glVertex3f(-0.4f,-0.2f,0.0f);

		glVertex3f(-0.4f,-0.2f,0.0f);
		glVertex3f(0.4f,-0.2f,0.0f);

		glVertex3f(0.4f,-0.2f,0.0f);
		glVertex3f(0.4f,-1.0f,0.0f);

	glEnd();
	view.endGL();

}

bool areaLightNode::isBounded() const
{
	return false;
}