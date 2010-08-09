#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bCmdRenderAnimation.h"
#include "bCmdRender.h"
#include"bReadShader.h"
#include"bReadObject.h"
#include"bReadLight.h"
#include"bReadCamera.h"
#include "bReadRender.h"
#include "bReadBackground.h"

#include <maya/MGlobal.h>
#include <maya/MRenderView.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include<interface/yafrayinterface.h>
#include <yafraycore/memoryIO.h>
#include<map>
#include<string>
using namespace yafaray;
using namespace std;
std::map<string , yafaray::material_t*> renderAnimation::materialMap;

MStatus renderAnimation::doIt(const MArgList &args)
{
	MStatus stat;
	yafrayInterface_t * yafAnimation=renderScene::getyI();

	return stat;
}