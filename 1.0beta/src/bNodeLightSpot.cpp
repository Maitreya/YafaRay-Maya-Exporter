#define NOMINMAX
#define _USE_MATH_DEFINES 1
#define PI 3.1415
#include "bNodeLightSpot.h"

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

const MTypeId spotLightNode::id(0x75312);
MObject spotLightNode::spotConeAngle;
MObject spotLightNode::spotBlend;
MObject spotLightNode::spotSoftShadows;
MObject spotLightNode::spotShadowFuzzyness;
MObject spotLightNode::spotPhotonOnly;
MObject spotLightNode::spotSamples;
MObject spotLightNode::spotColor;
MObject spotLightNode::spotPower;

void* spotLightNode::creator()
{
	return new spotLightNode;
}
MStatus spotLightNode::initialize()
{
	MStatus stat;
	MFnNumericAttribute numAttr;

	spotColor=numAttr.createColor("LightColor","lico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	spotPower=numAttr.create("Power","pw",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	spotConeAngle=numAttr.create("ConeAngle","ca",MFnNumericData::kFloat,30.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(1.0);
	numAttr.setMax(90.0); 

	spotBlend=numAttr.create("PenumbraDistance","pd",MFnNumericData::kFloat,0.5);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0); 

	spotSoftShadows=numAttr.create("SoftShadows","ss",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	spotShadowFuzzyness=numAttr.create("ShadowFuzzyness","sf",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(1.0); 

	spotPhotonOnly=numAttr.create("PhotonOnly","po",MFnNumericData::kBoolean,0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);

	spotSamples=numAttr.create("Samples","sam",MFnNumericData::kInt,16);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0);
	numAttr.setMax(512); 

	addAttribute(spotColor);
	addAttribute(spotPower);
	addAttribute(spotConeAngle);
	addAttribute(spotBlend);
	addAttribute(spotPhotonOnly);
	addAttribute(spotSoftShadows);
	addAttribute(spotShadowFuzzyness);
	addAttribute(spotSamples);

	return stat;
}
void spotLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	MFnDagNode spotFn(thisMObject());
	float coneAngle,blend;

	MPlug coneAnglePlug=spotFn.findPlug(spotConeAngle);
	coneAnglePlug.getValue(coneAngle);
	MPlug blendPlug=spotFn.findPlug(spotBlend);
	blendPlug.getValue(blend);

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
	//MPoint sharp(0.0,0.0,0.0);
	short spotLength=2;
	float coneArc=coneAngle*PI/180;
	float zArc=60*PI/180;
	MFloatVectorArray points(6);
	points[0].x=-2*sin(coneArc);
	points[0].y=-2*cos(coneArc);
	points[0].z=0.0;

	points[1].x=-2*sin(coneArc)*cos(zArc);
	points[1].y=points[0].y;
	points[1].z=2*sin(coneArc)*sin(zArc);

	points[2].x=-points[1].x;
	points[2].y=points[0].y;
	points[2].z=points[1].z;

	points[3].x=-points[0].x;
	points[3].y=points[0].y;
	points[3].z=0.0;

	points[4].x=-points[1].x;
	points[4].y=points[0].y;
	points[4].z=-points[1].z;

	points[5].x=points[1].x;
	points[5].y=points[0].y;
	points[5].z=-points[1].z;


	view.beginGL();
	glBegin(GL_LINES);
	   for (unsigned int i=0;i<6;i++)
	   {
		   glVertex3f(0.0,0.0,0.0);
		   glVertex3f(points[i].x, points[i].y, points[i].z);
	   }

	   glVertex3f(0.0,0.0,0.0);
	   glVertex3f(0.0,-3.0,0.0);

	   glVertex3f(0.0,-3.0,0.0);
	   glVertex3f(0.1,-2.8,0.0);

	   glVertex3f(0.0,-3.0,0.0);
	   glVertex3f(-0.1,-2.8,0.0);

   glEnd();
   glBegin(GL_LINE_LOOP);
	   for (unsigned int i=0;i<6;i++)
	   {
		   glVertex3f(points[i].x, points[i].y, points[i].z);
	   }
   glEnd();
   glBegin(GL_LINE_LOOP);
   for (unsigned int i=0;i<6;i++)
   {
	   glVertex3f(blend*(points[i].x), points[i].y, blend*(points[i].z));
   }
   glEnd();
   view.endGL();
}
bool spotLightNode::isBounded() const
{
	return false;
}