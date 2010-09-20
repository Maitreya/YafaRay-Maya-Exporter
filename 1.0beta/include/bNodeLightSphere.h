#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif
#include <iostream>
#include<maya/MPxLocatorNode.h>
#include<maya/MTypeId.h>
#include<maya/M3dView.h>


#ifndef SPHERELIGHT
#define SPHERELIGHT

class  sphereLightNode : public MPxLocatorNode
{
public:
	//virtual MStatus compute( const MPlug& plug, MDataBlock& data );
	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style,M3dView::DisplayStatus status );
	virtual bool  isBounded() const;
	//    virtual MBoundingBox  boundingBox() const; 
	static  void * creator();
	static  MStatus  initialize();

	static const MTypeId id;
private:
	//light input
	static MObject sphereColor;
	static MObject spherePower;
	static MObject sphereRadius;
	static MObject sphereSamples;
	static MObject sphereMakeGeometry;

};
#endif