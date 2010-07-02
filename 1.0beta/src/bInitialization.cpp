#include"bWorldSettingNode.h"
#include"bRenderSettingNode.h"

#include<maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj,"YafaRay","2011","Any");

	stat=plugin.registerNode("worldSetting",worldSettingNode::id,worldSettingNode::creator,worldSettingNode::initialize);
	if(!stat) stat.perror("register node failed");

	stat=plugin.registerNode("renderSetting",renderSettingNode::id,renderSettingNode::creator,renderSettingNode::initialize);
	if(!stat) stat.perror("register node failed");

	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj);

	stat=plugin.deregisterNode(worldSettingNode::id);
	if(!stat) stat.perror("deregister node failed");

	stat=plugin.deregisterNode(renderSettingNode::id);
	if(!stat) stat.perror("deregister node failed");

	return MS::kSuccess;
}