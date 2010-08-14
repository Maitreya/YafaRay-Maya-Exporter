#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include<map>
#include<string>
#include<maya/MPxCommand.h>
#include<maya/MGlobal.h>
#include<interface/yafrayinterface.h>
using namespace yafaray;
using namespace std;

#ifndef RENDERANIMATION
#define RENDERANIMATION

class renderAnimation:public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList& args);
	static void *creator(){return new renderAnimation;}
private:
	MStatus beginRender(yafrayInterface_t &yI,const int sizex, const int sizey,const int currentTme);
	MStatus renderToImage(const int sizex, const int sizey, const float *imageM,const int currentTme);
	static std::map<string , yafaray::material_t*> materialMap;
	static std::map<string, yafaray::texture_t*> textureMap;
};
#endif