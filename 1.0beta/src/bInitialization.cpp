#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include"bNodeWorldSetting.h"
#include"bNodeRenderSetting.h"
#include"bNodeMaterialGlass.h"
#include "bNodeMaterialCoatedGlossy.h"
#include "bNodeMaterialShinyDiffuse.h"
#include "bNodeMaterialBlend.h"
#include "bNodeMaterialGlossy.h"
#include "bNodeMaterialRoughGlass.h"
#include"bCmdLoadPlugin.h"
#include"bNodeCameraAngular.h"
#include"bCmdCreateCameraAngular.h"
#include"bNodeLightPoint.h"
#include "bNodeLightArea.h"
#include "bNodeLightDirectional.h"
#include "bNodeLightSpot.h"
#include "bNodeLightSphere.h"
#include "bNodeLightSun.h"
#include"bCmdRender.h"
#include "bCmdRenderPreview.h"

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

	//materials
	stat=plugin.registerNode("yafGlass",glassNode::id,glassNode::creator,glassNode::initialize,MPxNode::kDependNode,&MaterialClassify);
	if(!stat) stat.perror("register node yafGlass failed");

	stat=plugin.registerNode("yafCoatedGlossy",coatedGlossyNode::id,coatedGlossyNode::creator,coatedGlossyNode::initialize,MPxNode::kDependNode,&MaterialClassify);
	if(!stat) stat.perror("register node yafCoatedGlossy failed");

	stat=plugin.registerNode("yafShinyDiffuse",shinyDiffuseNode::id,shinyDiffuseNode::creator,shinyDiffuseNode::initialize,MPxNode::kDependNode,&MaterialClassify);
	if(!stat) stat.perror("register node yafShinyDiffuse failed");

	stat=plugin.registerNode("yafBlend",blendNode::id,blendNode::creator,blendNode::initialize,MPxNode::kDependNode,&MaterialClassify);
	if(!stat) stat.perror("register node yaBlend failed");

	stat=plugin.registerNode("yafGlossy",glossyNode::id, glossyNode::creator, glossyNode::initialize, MPxNode::kDependNode, &MaterialClassify);
	if(!stat) stat.perror("register node yaGlossy failed");

	stat=plugin.registerNode("yafRoughGlass",roughGlassNode::id, roughGlassNode::creator, roughGlassNode::initialize, MPxNode::kDependNode, &MaterialClassify);
	if(!stat) stat.perror("register node yafRoughGlass failed");

	//cameras
	stat=plugin.registerNode("yafAngular",angularNode::id,angularNode::creator,angularNode::initialize);
	if(!stat) stat.perror("register node yafAngular failed");

	stat=plugin.registerCommand("yafCreateAngular",createAngular::creator);
	if(!stat) stat.perror("register command yafCreateAngular failed");

	//lights
	stat=plugin.registerNode("yafPointLight",pointLightNode::id,pointLightNode::creator,pointLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
    if(!stat) stat.perror("register node yafPointLight failed");

	stat=plugin.registerNode("yafDirectionalLight",directionalLightNode::id,directionalLightNode::creator,directionalLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
	if(!stat) stat.perror("register node yafDirectionalLight failed");

	stat=plugin.registerNode("yafAreaLight",areaLightNode::id,areaLightNode::creator,areaLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
	if(!stat) stat.perror("register node yafAreaLight failed");

	stat=plugin.registerNode("yafSpotLight",spotLightNode::id,spotLightNode::creator,spotLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
	if(!stat) stat.perror("register node yafSpotLight failed");

	stat=plugin.registerNode("yafSphereLight",sphereLightNode::id,sphereLightNode::creator,sphereLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
	if(!stat) stat.perror("register node yafSphereLight failed");

	stat=plugin.registerNode("yafSunLight",sunLightNode::id,sunLightNode::creator,sunLightNode::initialize,MPxNode::kLocatorNode,&LightClassify);
	if(!stat) stat.perror("register node yafSunLight failed");

	stat=plugin.registerCommand("yafRender",renderScene::creator);
	if(!stat) stat.perror("register command yafRender failed");

	stat=plugin.registerCommand("yafRenderPreview",renderPreview::creator);
	if(!stat) stat.perror("register command yafRenderPreview failed");

	stat=plugin.registerUI("yafCreateUI","yafDeleteUI");
	if(!stat) stat.perror("register UI failed");

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

	stat=plugin.deregisterNode(coatedGlossyNode::id);
	if(!stat) stat.perror("deregister node yafCoatedGlossy failed");

	stat=plugin.deregisterNode(shinyDiffuseNode::id);
	if(!stat) stat.perror("deregister node yafShinyDiffuse failed");

	stat=plugin.deregisterNode(blendNode::id);
	if(!stat) stat.perror("deregister node yafBlend failed");

	stat=plugin.deregisterNode(glossyNode::id);
	if(!stat) stat.perror("deregister node yafGlossy failed");

	stat=plugin.deregisterNode(roughGlassNode::id);
	if(!stat) stat.perror("deregister node yafRoughGlass failed");

	stat=plugin.deregisterNode(angularNode::id);
	if(!stat) stat.perror("deregister node yafAngular failed");

	stat=plugin.deregisterCommand("yafCreateAngular");
	if(!stat) stat.perror("deregister command yafCreateAngular failed");

	stat=plugin.deregisterNode(pointLightNode::id);
	if(!stat) stat.perror("deregister node yafPointLight failed");

	stat=plugin.deregisterNode(areaLightNode::id);
	if(!stat) stat.perror("deregister node yafAreaLight failed");

	stat=plugin.deregisterNode(directionalLightNode::id);
	if(!stat) stat.perror("deregister node yafDirectionalLight failed");

	stat=plugin.deregisterNode(spotLightNode::id);
	if(!stat) stat.perror("deregister node yafSpotLight failed");

	stat=plugin.deregisterNode(sphereLightNode::id);
	if(!stat) stat.perror("deregister node yafSphereLight failed");

	stat=plugin.deregisterNode(sunLightNode::id);
	if(!stat) stat.perror("deregister node yafSunLight failed");

	stat=plugin.deregisterCommand("yafRender");
	if(!stat) stat.perror("deregister command yafRender failed");

	stat=plugin.deregisterCommand("yafRenderPreview");
	if(!stat) stat.perror("deregister command yafRenderPreview failed");

	return stat;
}