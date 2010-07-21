#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
#include <string>
#include <map>
using namespace yafaray;
using namespace std;

#ifndef RENDERPREVIEW
#define RENDERPREVIEW
class renderPreview : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new renderPreview;}
private:
	MStatus toRenderView(const int size, const float * imageM);
	static std::map<string , yafaray::material_t*> materialMap;
};

#endif