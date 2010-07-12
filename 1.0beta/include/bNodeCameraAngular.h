#include<maya/MPxNode.h>
#include<maya/MTypeId.h>

#ifndef ANGULAR
#define ANGULAR

class angularNode : public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;
private:
	static MObject angularCircular;
    static MObject angularMirrored;
	static MObject angularMaxAngle;
	static MObject angularAngle;
	//this is the output attribute, used for connecting with the created camera
	static MObject angularOutAngular;
	

};
#endif