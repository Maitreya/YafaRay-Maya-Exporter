#define NOMINMAX
#define _USE_MATH_DEFINES 1
#ifndef BLENDNODE
#define BLENDNODE

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MStringArray.h>
class blendNode : public MPxNode
{
public:
    virtual MStatus compute(const MPlug &plug,MDataBlock &data);
    static void * creator();
    static MStatus initialize();
    static const MTypeId id;
	static MStatus getAllMaterial(MStringArray &resultArray);
private:
	static MObject materialA;
	static MObject materialB;
	static MObject diffuseTexLayer;
	static MObject blendValue;
	static MObject outBlend;

};

#endif