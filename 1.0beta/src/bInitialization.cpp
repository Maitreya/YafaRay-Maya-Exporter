#include"bNodeWorldSetting.h"
#include"bNodeRenderSetting.h"
#include"bNodeMaterialGlass.h"
//#include"bReadObjectCmd.h"
#include"bCmdLoadPlugin.h"
#include"bNodeCameraAngular.h"
#include"bCmdCreateCameraAngular.h"
#include"bNodeLightPoint.h"
#include"bCmdRender.h"

#include<maya/MFnPlugin.h>
#include<maya/MString.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
    const MString MaterialClassify("shader/surface");
	const MString LightClassify("light");

	MFnPlugin plugin(obj,"YafaRay","2011","Any");

	stat=plugin.registerCommand("yafLoad",loadPlugin::creator);
	if(!stat) stat.perror("register command yafLoad failed");

	stat=plugin.registerNode("yafWorldSetting",worldSettingNode::id,worldSettingNode::creator,worldSettingNode::initialize);
	if(!stat) stat.perror("register node worldSetting failed");

	stat=plugin.registerNode("yafRenderSetting",renderSettingNode::id,renderSettingNode::creator,renderSettingNode::initialize);
	if(!stat) stat.perror("register node renderSetting failed");

	stat=plugin.registerNode("yafGlass",glassNode::id,glassNode::creator,glassNode::initialize,MPxNode::kDependNode,&MaterialClassify);
	if(!stat) stat.perror("register node yafGlass failed");

//	stat=plugin.registerCommand("yafReadObject",readObject::creator);
//	if(!stat) stat.perror("register command readObject failed");
	stat=plugin.registerNode("yafAngular",angularNode::id,angularNode::creator,angularNode::initialize);
	if(!stat) stat.perror("register node yafAngular failed");

	stat=plugin.registerCommand("yafCreateAngular",createAngular::creator);
	if(!stat) stat.perror("register command yafCreateAngular failed");

	stat=plugin.registerNode("yafPointLight",pointLightNode::id,pointLightNode::creator,pointLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
    if(!stat) stat.perror("register node yafPointLight failed");

	stat=plugin.registerCommand("yafRender",renderScene::creator);
	if(!stat) stat.perror("register command yafRender failed");


	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj);

	stat=plugin.deregisterCommand("yafLoad");
	if(!stat) stat.perror("deregister command yafLoad failed");

	stat=plugin.deregisterNode(worldSettingNode::id);
	if(!stat) stat.perror("deregister node worldSetting failed");

	stat=plugin.deregisterNode(renderSettingNode::id);
	if(!stat) stat.perror("deregister node renderSetting failed");

	stat=plugin.deregisterNode(glassNode::id);
	if(!stat) stat.perror("deregister node yafGlass failed");

//	stat=plugin.deregisterCommand("yafReadObject");
//	if(!stat) stat.perror("deregister command readObject failed");

	stat=plugin.deregisterNode(angularNode::id);
	if(!stat) stat.perror("deregister node yafAngular failed");

	stat=plugin.deregisterCommand("yafCreateAngular");
	if(!stat) stat.perror("deregister command yafCreateAngular failed");

	stat=plugin.deregisterNode(pointLightNode::id);
	if(!stat) stat.perror("deregister node yafPointLight failed");

	stat=plugin.deregisterCommand("yafRender");
	if(!stat) stat.perror("deregister command yafRender failed");

	return stat;
}