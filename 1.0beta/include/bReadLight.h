#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include<interface/yafrayinterface.h>
#include<maya/MGlobal.h>
using namespace yafaray;

#ifndef READLIGHT
#define READLIGHT
class getLight
{
public:
	MStatus readLight(yafrayInterface_t &yI);
};
#endif