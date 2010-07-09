#include"bWorldSettingNode.h"
#include"bRenderSettingNode.h"
#include"bGlassNode.h"
#include"bReadObjectCmd.h"
#include"bLoadPluginCmd.h"

#include<maya/MFnPlugin.h>
#include<maya/MString.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
    const MString UserClassify("shader/surface");

	MFnPlugin plugin(obj,"YafaRay","2011","Any");

	stat=plugin.registerCommand("yafLoad",loadPlugin::creator);
	if(!stat) stat.perror("register command yafLoad failed");

	stat=plugin.registerNode("yafWorldSetting",worldSettingNode::id,worldSettingNode::creator,worldSettingNode::initialize);
	if(!stat) stat.perror("register node worldSetting failed");

	stat=plugin.registerNode("yafRenderSetting",renderSettingNode::id,renderSettingNode::creator,renderSettingNode::initialize);
	if(!stat) stat.perror("register node renderSetting failed");

	stat=plugin.registerNode("yafGlass",glassNode::id,glassNode::creator,glassNode::initialize,MPxNode::kDependNode,&UserClassify);
	if(!stat) stat.perror("register node yafGlass failed");

	stat=plugin.registerCommand("yafReadObject",readObject::creator);
	if(!stat) stat.perror("register command readObject failed");


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

	stat=plugin.deregisterCommand("yafReadObject");
	if(!stat) stat.perror("deregister command readObject failed");




	return stat;
}