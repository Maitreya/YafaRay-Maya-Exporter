#include<interface/yafrayinterface.h>
#include<maya/MGlobal.h>
using namespace yafaray;

#ifndef READCAMERA
#define READCAMERA

class getCamera
{
public:
	MStatus readCamera(yafrayInterface_t &yI);
};

#endif