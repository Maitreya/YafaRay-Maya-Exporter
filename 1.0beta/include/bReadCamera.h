#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif

#include <iostream>

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