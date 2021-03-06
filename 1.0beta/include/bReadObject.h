#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif
#include <iostream>
#include<maya/MGlobal.h>
#include<interface/yafrayinterface.h>
#include<map>
#include<string>
using namespace std;
using namespace yafaray;

#ifndef READOBJECT
#define READOBJECT

class getObject
{
public:
//	getObject();
//	virtual ~getObject();
	MStatus readObject( yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap);
private:
	MStatus readMesh(yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap);
	MStatus readParticle(yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap);

};

#endif