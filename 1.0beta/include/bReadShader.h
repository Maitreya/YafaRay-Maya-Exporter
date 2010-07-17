#include<maya/MGlobal.h>
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
	MStatus readShader(yafrayInterface_t &yI,std::map< string ,yafaray::material_t *> &materialMap);
};
#endif