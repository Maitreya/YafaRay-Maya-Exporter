#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include<interface/yafrayinterface.h>
#include<maya/MGlobal.h>
#include<map>
#include<string>
using namespace yafaray;

#ifndef READLIGHT
#define READLIGHT
class getLight
{
public:
	MStatus readLight(yafrayInterface_t &yI);
private:
	int makeSphere(yafrayInterface_t &yI,int u, int v, double x,double y,double z,float radius,yafaray::material_t * lightMat);
};
#endif