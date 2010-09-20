#define NOMINMAX
#define _USE_MATH_DEFINES 1

#ifdef WIN32
#pragma once
#define NT_PLUGIN
#endif
#include <iostream>
#include<maya/MGlobal.h>
#include <maya/MPlug.h>
#include<map>
#include<string>
#include<interface/yafrayinterface.h>
using namespace yafaray;
using namespace std;

#ifndef READSHADER
#define READSHADER

class getShader
{
public:
	//a class used to scan the scene
	//find and create yafaray material,get a pointer material_t*
	//put the pointer into a std::map parameter
	//textrue not considered yet
	//getShader();
	//virtual ~getShader();
	MStatus readShader(yafrayInterface_t &yI,std::map< string ,yafaray::material_t *> &materialMap, std::map<string, yafaray::texture_t *> &textureMap);
	MStatus readTexture(yafrayInterface_t &yI, std::map<string, yafaray::texture_t*> &textureMap);
private:
	MStatus nodeType(MPlug srcPlug,MString &nType,MString &nName);
	//layerShader: what will be linked to the apropiate slot of material
	MStatus readTexLayer(yafrayInterface_t &yI, MString textureLayerName, std::map<string,yafaray::texture_t*> &textureMap , MObject color,MString &layerShader);
	MStatus readLayerMap(yafrayInterface_t &yI, MString texName, MString texType, MObject color, MString upperLayer="");


};
#endif