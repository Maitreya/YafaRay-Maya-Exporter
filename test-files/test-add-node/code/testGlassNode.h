#ifndef TESTNODE
#define TESTNODE

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class testGlassNode: public MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static MObject absorbColor;
	static MObject absorbDistance;
	static MObject filterColor;
	static MObject mirrorColor;
	static MObject IOR;
	static MObject transmitFilter;
	static MObject dispersionPower;
	static MObject fakeShadows;
	static MObject outGlass;


};
#endif