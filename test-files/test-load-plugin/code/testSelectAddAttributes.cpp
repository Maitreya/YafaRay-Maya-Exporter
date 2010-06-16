#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>

#ifndef TEST
#define TEST
//#program once

class testSelectAddAttribute: public MPxCommand
{
public:
	virtual MStatus doIt( const MArgList& args );;
	static void *creator(){return new testSelectAddAttribute;}
};




MStatus testSelectAddAttribute::doIt( const MArgList& args )

{
	MDagPath node;
	MObject component;
	MSelectionList list;
	MFnDagNode nodeFn;
	MGlobal::getActiveSelectionList( list );
	for ( unsigned int index = 0; index < list.length(); index++ )
	{
		list.getDagPath( index, node, component );
		nodeFn.setObject( node );
		cout<<nodeFn.name().asChar( ) << "is selected" << endl;
	}
	return MS::kSuccess;
}



