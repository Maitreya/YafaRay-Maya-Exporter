#define NOMINMAX
#define _USE_MATH_DEFINES 1

#include "bNodeLightDirectional.h"

#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MColor.h>

const MTypeId directionalLightNode::id(0x75309);
MObject directionalLightNode::directionalLightColor;
MObject directionalLightNode::directionalPower;
MObject directionalLightNode::directionalInfinite;
MObject directionalLightNode::directionalRadius;

void* directionalLightNode::creator()
{
	return new directionalLightNode;
}
MStatus directionalLightNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;

	directionalLightColor=numAttr.createColor("LightColor","lico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	directionalPower=numAttr.create("Power","dp",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	directionalInfinite=numAttr.create("Infinite","di",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	directionalRadius=numAttr.create("Radius","dr",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	addAttribute(directionalLightColor);
	addAttribute(directionalPower);
	addAttribute(directionalInfinite);
	addAttribute(directionalRadius);
	

	return stat;
}
void directionalLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
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
			glVertex3f(0.0f,0.0f,2.0f);
			glVertex3f(0.0f,0.0f,-2.0f);
	glEnd();
	glBegin(GL_TRIANGLES);
			glVertex3f(-0.2f,0.0f,-1.6f);
			glVertex3f(0.0f,0.0f,-2.0f);
			glVertex3f(0.2f,0.0f,-1.6f);

			glVertex3f(-0.2f,0.0f,-1.1f);
			glVertex3f(0.0f,0.0f,-1.5f);
			glVertex3f(0.2f,0.0f,-1.1f);

			glVertex3f(-0.2f,0.0f,-0.6f);
			glVertex3f(0.0f,0.0f,-1.0f);
			glVertex3f(0.2f,0.0f,-0.6f);

	glEnd();
	
	view.endGL();
}
bool directionalLightNode::isBounded() const
{
	return false;
}