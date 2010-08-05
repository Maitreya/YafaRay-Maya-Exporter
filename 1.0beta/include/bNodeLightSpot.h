#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include<maya/MPxLocatorNode.h>
#include<maya/MTypeId.h>
#include<maya/M3dView.h>


#ifndef SPOTLIGHT
#define SPOTLIGHT

class  spotLightNode : public MPxLocatorNode
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
	static MObject spotConeAngle;
	static MObject spotBlend;
	static MObject spotSoftShadows;
	static MObject spotShadowFuzzyness;
	static MObject spotPhotonOnly;
	static MObject spotSamples;
	static MObject spotColor;
	static MObject spotPower;

};
#endif