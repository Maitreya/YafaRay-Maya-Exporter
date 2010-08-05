#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeLightPoint.h"

#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
//#include <maya/MDataHandle.h>
//#include <maya/MDistance.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MColor.h>

const MTypeId pointLightNode::id(0x75304);

MObject pointLightNode::pointLightColor;
MObject pointLightNode::pointPower;


void * pointLightNode::creator()
{
	return new pointLightNode();
}
MStatus pointLightNode::initialize()
{
	MStatus stat;

	MFnNumericAttribute numAttr;

	pointLightColor=numAttr.createColor("LightColor","plico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(1.0,1.0,1.0);

	pointPower=numAttr.create("Power","pp",MFnNumericData::kFloat,1.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0); 

	addAttribute(pointLightColor);
    addAttribute(pointPower);

	return stat=MStatus::kSuccess;

}
//MStatus pointLightNode::compute(const MPlug &plug, MDataBlock &data)
//{
//	MStatus stat;
//	return stat=MStatus::kSuccess;
//}
void pointLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
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
	glBegin(GL_LINES);
	                        glVertex3f(0.0f,0.1f,0.0f);
		                    glVertex3f(0.1f,0.0f,0.0f);

							glVertex3f(0.0f,0.1f,0.0f);
		                    glVertex3f(-0.1f,0.0f,0.0f);

							glVertex3f(0.0f,0.1f,0.0f);
		                    glVertex3f(0.0f,0.0f,0.1f);

							glVertex3f(0.0f,0.1f,0.0f);
		                    glVertex3f(0.0f,0.0f,-0.1f);

							glVertex3f(0.0f,-0.1f,0.0f);
		                    glVertex3f(0.1f,0.0f,0.0f);

							glVertex3f(0.0f,-0.1f,0.0f);
		                    glVertex3f(-0.1f,0.0f,0.0f);

							glVertex3f(0.0f,-0.1f,0.0f);
		                    glVertex3f(0.0f,0.0f,0.1f);

							glVertex3f(0.0f,-0.1f,0.0f);
		                    glVertex3f(0.0f,0.0f,-0.1f);

							glVertex3f(-0.1f,0.0f,0.0f);
		                    glVertex3f(0.0f,0.0f,0.1f);

							glVertex3f(0.0f,0.0f,0.1f);
		                    glVertex3f(0.1f,0.0f,0.0f);

							glVertex3f(0.1f,0.0f,0.0f);
		                    glVertex3f(0.0f,0.0f,-0.1f);

							glVertex3f(0.0f,0.0f,-0.1f);
		                    glVertex3f(-0.1f,0.0f,0.0f);


    glEnd();
	view.endGL();
}
bool pointLightNode::isBounded() const
{
	return false;
}
//MBoundingBox pointLightNode::boundingBox() const
//{
//      MPoint corner1(-1.1, 0.0, -1.1);
//      MPoint corner2(1.1, 0.0, 1.1);
//
//	  return MBoundingBox(corner1,corner2);
//
//}
