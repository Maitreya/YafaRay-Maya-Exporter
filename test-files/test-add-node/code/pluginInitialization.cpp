#include"testGlassNode.h"
#include"testGlassCmd.h"
#include<maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj,"YafaRay","2011","Any");

	stat=plugin.registerNode("testGlassNode",testGlassNode::id,testGlassNode::creator,testGlassNode::initialize);
	if(!stat) stat.perror("register node failed");

	stat=plugin.registerCommand("testGlass",testGlassCmd::creator);
	if(!stat) stat.perror("register command failed");

	stat=plugin.registerUI("testMaterialNodeCreateUI","testMaterialNodeDeleteUI");
	if(!stat) stat.perror("register UI failed");

	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin plugin(obj);

	stat=plugin.deregisterNode(testGlassNode::id);
	if(!stat) stat.perror("deregister node failed");

	stat=plugin.deregisterCommand("testGlass");
	if(!stat) stat.perror("deregister command failed");

	return MS::kSuccess;
}