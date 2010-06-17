#include"testGlassCmd.h"
#include"testGlassNode.h"

#include<maya/MGlobal.h>
#include<maya/MString.h>
#include<maya/MSelectionList.h>
#include<maya/MItSelectionList.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MFnNumericAttribute.h>
#include<maya/MDGModifier.h>
#include<maya/MPlug.h>
#include<maya/MPlugArray.h>




MStatus testGlassCmd::doIt(const MArgList &args)
{
	MStatus stat=MStatus::kSuccess;

	MSelectionList list;
	MFnDependencyNode meshDependFn;
	MFnDependencyNode materialDependFn;
	MDGModifier dgMod;

	//create a new material node, here is the test glass node
	MObject testGNode=dgMod.createNode(testGlassNode::id);
	materialDependFn.setObject(testGNode);
	MString testName=materialDependFn.name();
	MGlobal::displayInfo(testName+" god, please give me the node first time");


	//find the mesh node(s) from selected object(s)
	//create a new "yafaray material" attribute if the mesh node(s) dont have
	//then conect the material node to the mesh node(s)
	MGlobal::getActiveSelectionList( list );
	MItSelectionList iter( list, MFn::kMesh );
	for( ; !iter.isDone(); iter.next())
	{
		MObject meshDependNode;
		MFnNumericAttribute numAttr;
		

		iter.getDependNode( meshDependNode );
		meshDependFn.setObject( meshDependNode );
		//MString dependName=dependFn.name();
		//MGlobal::displayInfo(dependName+" is here\n");

		if( !meshDependFn.hasAttribute("YafarayMaterial") )
		{
			MObject attrYafarayMaterial=numAttr.create("YafarayMaterial","yama",MFnNumericData::kBoolean);
			numAttr.setDefault( true );
			numAttr.setStorable( true );
			meshDependFn.addAttribute(attrYafarayMaterial,MFnDependencyNode::kLocalDynamicAttr);
		}

        //find the source plug and the result plug, then connect them
		//if the result plug as  already had a sourse plug connected to it, disconnect first
		MPlug glassPlug=materialDependFn.findPlug("OutGlass");
		MPlug meshPlug=meshDependFn.findPlug("YafarayMaterial");

		MPlugArray srcMeshPlug;		
		if(meshPlug.connectedTo(srcMeshPlug, true, false))
		{
			MPlug srcPlug;
			//if(srcMeshPlug.length!=0) 
			//{
				srcPlug=srcMeshPlug[0];
			//}

			dgMod.disconnect(srcPlug, meshPlug);
		}

		dgMod.connect(glassPlug,meshPlug);

	}



	dgMod.doIt();
	//why still cant got the name here?
	MGlobal::displayInfo(testName+" god, please give me the node second time");
	
	return stat;
}