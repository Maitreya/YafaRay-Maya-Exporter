#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bCmdRender.h"
#include"bReadShader.h"
#include"bReadObject.h"
#include"bReadLight.h"
#include"bReadCamera.h"
#include "bReadRender.h"
#include "bReadBackground.h"

#include <maya/MGlobal.h>
#include<interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
#include<map>
#include<string>
using namespace yafaray;
//using namespace std;
yafrayInterface_t renderScene::yI;
std::map<string , yafaray::material_t*> renderScene::materialMap;
MStatus renderScene::doIt(const MArgList &args)
{
	MStatus stat=MStatus::kSuccess;
	yI.startScene();

	getShader shader;
	shader.readShader(yI,materialMap);
	cout<<"shader ok"<<endl;

	getObject object;
	object.readObject(yI,materialMap);
	cout<<"obj ok"<<endl;

	getLight light;
	light.readLight(yI);
	cout<<"light ok"<<endl;

	getCamera camera;
	camera.readCamera(yI);
	cout<<"camera ok"<<endl;

	getWorld world;
	world.readWorld(yI);
	cout<<"world ok"<<endl;

	getRender renderSetting;
	renderSetting.readRender(yI);
	cout<<"renderSetting ok"<<endl;

//	beginRender();
//	MGlobal::displayInfo("yafaray render out");

	return stat;
}
//MStatus renderScene::beginRender()
//{
//	MStatus stat;
//	int sizex=640;
//	int sizey=480;
//
//
//	yafaray::memoryIO_t memoryIO(sizex,sizey,imageM);
//	yI.render(memoryIO);
//
////	delete [] imageM;
//
//	return stat;
//}
yafrayInterface_t* renderScene::getyI()
{
	return &yI;
}