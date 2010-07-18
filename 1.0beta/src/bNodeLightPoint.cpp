#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bNodeLightPoint.h"
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
//#include <maya/MDataHandle.h>
//#include <maya/MDistance.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MPoint.h>
#include <maya/MGlobal.h>
#include <maya/MColor.h>

const MTypeId pointLightNode::id(0x75304);

MObject pointLightNode::pointLightColor;
MObject pointLightNode::pointPower;

MObject pointLightNode::pointLightData;

MObject pointLightNode:: pointLightDirection;
MObject pointLightNode:: pointLightIntensity;
MObject pointLightNode:: pointLightAmbient;
MObject pointLightNode:: pointLightDiffuse;
MObject pointLightNode:: pointLightSpecular;
MObject pointLightNode:: pointLightShadowFraction;
MObject pointLightNode:: pointPreShadowIntensity;
MObject pointLightNode:: pointLightBlindData;

void * pointLightNode::creator()
{
	return new pointLightNode();
}
MStatus pointLightNode::initialize()
{
	MStatus stat;

	MFnNumericAttribute numAttr;
	MFnLightDataAttribute lightAttr;

	pointLightColor=numAttr.createColor("LightColor","plico");
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setDefault(0.0,0.3,0.2);

	pointPower=numAttr.create("Power","pp",MFnNumericData::kFloat,0.0);
	numAttr.setKeyable(true);
	numAttr.setStorable(true);
	numAttr.setMin(0.0);
	numAttr.setMax(10000.0);
    
	//outputs
	pointLightDirection = numAttr.createPoint( "lightDirection", "ld" );
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( numAttr.setDefault(-1.0f, 0.0f, 0.0f) );

    pointLightIntensity = numAttr.createColor( "lightIntensity", "li" );
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( numAttr.setDefault(1.0f, 0.5f, 0.2f) );

    pointLightAmbient = numAttr.create( "lightAmbient", "la", MFnNumericData::kBoolean);
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    numAttr.setDefault(true);

    pointLightDiffuse = numAttr.create( "lightDiffuse", "ldf",MFnNumericData::kBoolean);
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( numAttr.setDefault(true) );

    pointLightSpecular = numAttr.create( "lightSpecular", "ls", MFnNumericData::kBoolean);
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( numAttr.setDefault(true) );

    pointLightShadowFraction = numAttr.create("lightShadowFraction","lsf",MFnNumericData::kFloat);
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( numAttr.setDefault(0.0f) );

    pointPreShadowIntensity = numAttr.create("preShadowIntensity","psi",MFnNumericData::kFloat);
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( numAttr.setDefault(0.0f) );

    pointLightBlindData = numAttr.createAddr("lightBlindData","lbld");
    CHECK_MSTATUS ( numAttr.setStorable(false) );
    CHECK_MSTATUS ( numAttr.setHidden(true) );
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );

    pointLightData = lightAttr.create( "lightData", "ltd", pointLightDirection, pointLightIntensity, pointLightAmbient, 
		                                             pointLightDiffuse, pointLightSpecular, pointLightShadowFraction,pointPreShadowIntensity, pointLightBlindData);
    CHECK_MSTATUS ( numAttr.setReadable(true) );
    CHECK_MSTATUS ( numAttr.setWritable(false) );
    CHECK_MSTATUS ( lightAttr.setStorable(false) );
    CHECK_MSTATUS ( lightAttr.setHidden(true) );
    lightAttr.setDefault(-1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.2f, true, true,true, 0.0f, 1.0f, NULL);

	addAttribute(pointLightColor);
    addAttribute(pointPower);
    addAttribute(pointLightData);

	attributeAffects(pointLightColor,pointLightData);
    attributeAffects(pointPower,pointLightData);

    attributeAffects(pointLightData,pointLightData);
    attributeAffects( pointLightDirection,pointLightData);
    attributeAffects( pointLightIntensity,pointLightData);
    attributeAffects( pointLightAmbient,pointLightData);
    attributeAffects( pointLightDiffuse,pointLightData);
    attributeAffects( pointLightSpecular,pointLightData);
    attributeAffects( pointLightShadowFraction,pointLightData);
    attributeAffects( pointPreShadowIntensity,pointLightData);
    attributeAffects( pointLightBlindData,pointLightData);



	return stat=MStatus::kSuccess;

}
MStatus pointLightNode::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus stat;
	return stat=MStatus::kSuccess;
}
void pointLightNode::draw(M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	view.beginGL();
	        if ( ( style == M3dView::kFlatShaded ) || ( style == M3dView::kGouraudShaded ) ) 
                { 
                       glPushAttrib( GL_CURRENT_BIT );
                       if ( status == M3dView::kActive ) {
                             view.setDrawColor( 13, M3dView::kActiveColors );
                          } 
					   else 
					   {
						   MColor actColor(1.0,0.0,0.0,1);

                        view.setDrawColor(actColor);
                       }  
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

                       glPopAttrib();
                 }
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
