#define NOMINMAX
#define _USE_MATH_DEFINES 1
#define PI 3.1415
#include "bNodeLightSun.h"

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

const MTypeId sunLightNode::id(0x75314);

MObject sunLightNode::sunColor;
MObject sunLightNode::sunPower;
MObject sunLightNode::sunSamples;
MObject sunLightNode::sunAngle;

void * sunLightNode::creator()
{
	return new sunLightNode;
}
MStatus sunLightNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;

	sunColor=numAttr.createColor("LightColor","lico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	sunPower=numAttr.create("Power","pw",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	sunSamples=numAttr.create("Samples","sam",MFnNumericData::kInt,16);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0);
	numAttr.setMax(512); 

	sunAngle=numAttr.create("Angle","an",MFnNumericData::kFloat,0.5);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(80.0); 

	addAttribute(sunColor);
	addAttribute(sunPower);
	addAttribute(sunSamples);
	addAttribute(sunAngle);
	

	return stat;
}
void sunLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
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
	view.beginGL();
	float sunRadius=3.0;
	MFloatVectorArray points(36);
	for (int i=0; i<36; i++)
	{
		float sunArc=i*10*PI/180;
		points[i].x=sunRadius*sin(sunArc);
		points[i].y=sunRadius*cos(sunArc);
		points[i].z=0.0;
	}
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<36;i++)
	{
		glVertex3f(points[i].x,points[i].y,0.0);
	}
	glEnd();

	MFloatVectorArray pointsLines1(12);
	for (int i=0; i<12; i++)
	{
		float sunArc=i*30*PI/180;
		pointsLines1[i].x=sunRadius*sin(sunArc);
		pointsLines1[i].y=sunRadius*cos(sunArc);
		pointsLines1[i].z=0.0;
	}

	MFloatVectorArray pointsLines2(12);
	for (int i=0; i<12; i++)
	{
		float sunArc=i*30*PI/180;
		pointsLines2[i].x=(sunRadius+1)*sin(sunArc);
		pointsLines2[i].y=(sunRadius+1)*cos(sunArc);
		pointsLines2[i].z=0.0;
	}
	glBegin(GL_LINES);
	for(int i=0;i<12;i++)
	{
		glVertex3f(pointsLines1[i].x,pointsLines1[i].y,0.0);
		glVertex3f(pointsLines2[i].x,pointsLines2[i].y,0.0);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(0.5f,-1.0f,0.0f);
	    glVertex3f(0.0f,-2.0f,0.0f);
		glVertex3f(-0.5f,-1.0f,0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3f(1.8f,1.5f,0.0f);
	    glVertex3f(0.7f,0.5f,0.0f);
		glVertex3f(1.8f,-0.5f,0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex3f(-1.8f,1.5f,0.0f);
		glVertex3f(-0.7f,0.5f,0.0f);
		glVertex3f(-1.8f,-0.5f,0.0f);
	glEnd();

	glBegin(GL_LINES);
	    glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,3.0f);

		glVertex3f(0.0f,0.0f,3.0f);
		glVertex3f(0.0f,0.8f,2.6f);

		glVertex3f(0.0f,0.0f,3.0f);
		glVertex3f(0.0f,-0.8f,2.6f);
	glEnd();

	view.endGL();
}
bool sunLightNode::isBounded() const
{
	return false;
}