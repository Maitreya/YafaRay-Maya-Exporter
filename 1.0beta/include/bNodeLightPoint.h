#include<maya/MPxLocatorNode.h>
#include<maya/MTypeId.h>
#include<maya/M3dView.h>


#ifndef POINTLIGHT
#define POINTLIGHT

class pointLightNode : public MPxLocatorNode
{
public:
	virtual MStatus compute( const MPlug& plug, MDataBlock& data );
	virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style,M3dView::DisplayStatus status );
    virtual bool  isBounded() const;
//    virtual MBoundingBox  boundingBox() const; 
    static  void * creator();
    static  MStatus  initialize();

	static const MTypeId id;
private:
	//light input
	static MObject pointLightColor;
	static MObject pointPower;

	//only one output enough?
	static MObject pointLightData;
	//seems not....
	 static MObject  pointLightDirection;
     static MObject  pointLightIntensity;
     static MObject  pointLightAmbient;
     static MObject  pointLightDiffuse;
     static MObject  pointLightSpecular;
     static MObject  pointLightShadowFraction;
     static MObject  pointPreShadowIntensity;
     static MObject  pointLightBlindData;



};
#endif