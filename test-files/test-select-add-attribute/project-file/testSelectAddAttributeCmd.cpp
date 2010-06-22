//this can always add a "Yafaray" attribute to a transform node
//maybe can add to a shape node? test it when needed....
//shape node cant be edited here, hmm......
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>
#include <maya/MFnNumericAttribute.h>
#include "testSelectAddAttribute.h"





MStatus testSelectAddAttribute::doIt( const MArgList& args )

{
	MStatus stat;
	MDagPath dagPath;
	MObject component;
	MSelectionList list;
	//MFnAttribute attr;
	MFnDagNode nodeFn;
	MFnNumericAttribute numFn;
	MFnDependencyNode dependFn;
	


	MGlobal::getActiveSelectionList( list );

	for ( unsigned int index = 0; index < list.length(); index++ )
	{


		//this just get the name of the transform node
		MObject selectDepend;
		list.getDependNode( index,selectDepend );
		dependFn.setObject( selectDepend );

		MString dependName=dependFn.name();
		MGlobal::displayInfo(dependName+" is here!");

		MString dependType=dependFn.typeName();
		MGlobal::displayInfo(dependType+" is selected!");

		if( (dependType=="lambert")&&(!dependFn.hasAttribute("YafarayTest")) )
		{
			MObject attrYafaray=numFn.create("YafarayTest","Yttt",MFnNumericData::kBoolean);
			numFn.setDefault( true );
			CHECK_MSTATUS(numFn.setStorable(true));
		    dependFn.addAttribute(attrYafaray,MFnDagNode::kLocalDynamicAttr);
		
		}






	}

	return stat=MS::kSuccess;
}
////////////////////////////////remains////////////////////////////////

	//	list.getDagPath( index, dagPath, component );

	//	nodeFn.setObject(dagPath);
	//	MString selectedObj=nodeFn.name();
	//	

	//	//this is right, but i dont want it
	//	//if(!nodeFn.hasAttribute("YafarayTest",&stat))
	//	//{
	//	//	MGlobal::executeCommand("addAttr -longName ""YafarayTest""  -attributeType bool -defaultValue true "+selectedObj );
	//	//    
	//	//}

	//	//wow~~~~this is works too now, seems the error occurs because the name of the attribute mismatched
	//	if(!nodeFn.hasAttribute("YafarayTestToo"))
	//	{
	//		MObject attrYafaray=numFn.create("YafarayTestToo","Ytt",MFnNumericData::kFloat);
	//		numFn.setDefault(1.5);
	//		CHECK_MSTATUS(numFn.setStorable(true));
	//	    nodeFn.addAttribute(attrYafaray,MFnDagNode::kLocalDynamicAttr);
	//	

	//	}

	//	if(!nodeFn.hasAttribute("YafarayTestThree"))
	//	{
	//		MObject attrYafaray=numFn.create("YafarayTestThree","Yttt",MFnNumericData::kBoolean);
	//		numFn.setDefault( true );
	//		CHECK_MSTATUS(numFn.setStorable(true));
	//	    nodeFn.addAttribute(attrYafaray,MFnDagNode::kLocalDynamicAttr);
	//	

	//	}

		//if i use MFnAttribute, i should get MObject from the list
		//but MFnDagNode dont need MObject to setObject, dag node path is ok
		//MObject selectedObj=dagPath.node();
		
		//dependFn.setObject(selectedObj);
		//dependFn.addAttribute(attrYafaray,MFnDependencyNode::kLocalDynamicAttr)

		//attr.setObject(selectedObj);
		//still, i cant get out what is this getAddAttrCmd......
		//MString whatIsThis=attr.getAddAttrCmd();

		
		

		//here get the transform node of an MObject
	    //const char*  selectedDag=nodeFn.name().asChar();
		//MGlobal::displayInfo(selectedDag+" is selected");