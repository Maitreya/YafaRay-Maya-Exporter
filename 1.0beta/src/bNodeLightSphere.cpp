#define NOMINMAX
#define _USE_MATH_DEFINES 1
#define PI 3.1415
#include "bNodeLightSphere.h"

#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MColor.h>
#include <maya/MFnDagNode.h>
#include <maya/MFloatVectorArray.h>
#include <math.h>

const MTypeId sphereLightNode::id(0x75313);

MObject sphereLightNode::sphereColor;
MObject sphereLightNode::spherePower;
MObject sphereLightNode::sphereRadius;
MObject sphereLightNode::sphereSamples;
MObject sphereLightNode::sphereMakeGeometry;

void* sphereLightNode::creator()
{
	return new sphereLightNode;
}
MStatus sphereLightNode::initialize()
{
	MStatus stat;

	MFnNumericAttribute numAttr;

	sphereColor=numAttr.createColor("LightColor","lico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	spherePower=numAttr.create("Power","pw",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	sphereRadius=numAttr.create("Radius","ra",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(100.0); 

	sphereSamples=numAttr.create("Samples","sam",MFnNumericData::kInt,16);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0);
	numAttr.setMax(512); 

	sphereMakeGeometry=numAttr.create("MakeGeometry","mg",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	addAttribute(sphereColor);
	addAttribute(spherePower);
	addAttribute(sphereRadius);
	addAttribute(sphereSamples);
	addAttribute(sphereMakeGeometry);

	return stat;
}

void sphereLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	MFnDagNode sphereFn(thisMObject());
	MPlug radiusPlug=sphereFn.findPlug(sphereRadius);
	float radius;
	radiusPlug.getValue(radius);

	MColor drawColor;

	drawColor.set(MColor::kRGB,1,1,0);

	if (status==M3dView::kDormant)
	{
		drawColor.set(MColor::kRGB,1,0,0);
	}
	if (status==M3dView::kTemplate)
	{
		drawColor.set(MColor::kRGB,0,1,0);
	}
	view.setDrawColor(drawColor);
	MFloatVectorArray points(36);
	for (int i=0; i<36; i++)
	{
		float sphereArc=i*10*PI/180;
		points[i].x=radius*sin(sphereArc);
		points[i].y=radius*cos(sphereArc);
		points[i].z=0.0;
	}
	view.beginGL();
	glBegin(GL_LINE_LOOP);
	    for(int i=0;i<36;i++)
		{
			glVertex3f(points[i].x,points[i].y,0.0);
		}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for(int i=0;i<36;i++)
	{
		glVertex3f(points[i].x,(points[i].y)*cos(60*PI/180),(points[i].y)*sin(60*PI/180));
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for(int i=0;i<36;i++)
	{
		glVertex3f(points[i].x,(points[i].y)*cos(60*PI/180),-(points[i].y)*sin(60*PI/180));
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for(int i=0;i<36;i++)
	{
		glVertex3f((points[i].x)*sin(60*PI/180),points[i].y,(points[i].x)*cos(60*PI/180));
	}
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<36;i++)
	{
		glVertex3f(-(points[i].x)*sin(60*PI/180),points[i].y,(points[i].x)*cos(60*PI/180));
	}
	glEnd();

	view.endGL();



}
bool sphereLightNode::isBounded() const
{
	return false;
}