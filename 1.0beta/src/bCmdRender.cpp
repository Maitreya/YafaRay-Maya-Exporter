#include"bCmdRender.h"
#include"bReadShader.h"
#include"bReadObject.h"
#include"bReadLight.h"
#include"bReadCamera.h"

#include<interface/yafrayinterface.h>
#include<map>
#include<string>
using namespace yafaray;
using namespace std;

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
	


	return stat;
}