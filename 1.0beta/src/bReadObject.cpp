#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bReadObject.h"

#include<maya/MGlobal.h>
#include<maya/MSelectionList.h>
#include<maya/MItSelectionList.h>
#include<maya/MString.h>
#include<maya/MItMeshVertex.h>
#include<maya/MitMeshPolygon.h>
#include<maya/MPoint.h>
#include<maya/MPointArray.h>
#include<maya/MIntArray.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MFnMesh.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include<iostream>
#include<interface/yafrayinterface.h>
#include<map>
#include<string>
using namespace std;
using namespace yafaray;

MStatus getObject::readObject(yafaray::yafrayInterface_t &yI , std::map<string , yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;

	readMesh(yI,materialMap);
	MGlobal::displayInfo("read mesh succeeded!(*^__^*)");
	readParticle(yI,materialMap);
	

	return stat;
}
MStatus getObject::readMesh(yafaray::yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;

	MFnDependencyNode selectDepend;

	//select all the mesh in the scene
	MSelectionList selectList;
	MString matchPolygon("*");
	MGlobal::getSelectionListByName(matchPolygon,selectList);
	MItSelectionList selectMesh(selectList,MFn::kMesh);

	//this itorate should be done on only visiable meshes, but has not implement yet
	//how to find the visiability attribute of a mesh?
	for(; !selectMesh.isDone();selectMesh.next())
	{
		MObject originMesh;
		selectMesh.getDependNode(originMesh);

		selectDepend.setObject(originMesh);
		MString meshName=selectDepend.name();
		cout<<"the mesh name is "<<meshName.asChar()<<endl;

		//this is for getting the number of faces of the mesh
		MFnMesh meshFn(originMesh);
		
		//find the material of the mesh
		//not good method till now, use a stupid one.......yohohoho!!!found the way to do this!!
		MObjectArray sgArray;
		MIntArray indexSG;
		meshFn.getConnectedShaders(0, sgArray, indexSG);
		MFnDependencyNode sgFn(sgArray[0]);
		MPlug surfacePlug=sgFn.findPlug("surfaceShader");
		MPlugArray sourceArray;
		MString shaderName;
		if (surfacePlug.connectedTo(sourceArray, true, false))
		{
			MPlug sourcePlug=sourceArray[0];
			MObject sourceNode=sourcePlug.node();
			MFnDependencyNode sourceFn(sourceNode);
			shaderName=sourceFn.name();
		}

		string sName(shaderName.asChar());

		std::map<string , yafaray::material_t *>::iterator iter=materialMap.find(sName);
		if(iter==materialMap.end())
		{
			//here just a temp way
			//actually, if searched in the map and didn't find the shader name of this object(means this object didn't use yafaray's shader)
			//we should give it an default shader
			//to implement this, should export a default shader first, in the read shader step
			//implement this later
			MGlobal::displayError("iterator is useless");
			return MStatus::kFailure;
		}

		unsigned int yafID=yI.getNextFreeID();
		cout<<"the mesh id is "<<yafID<<endl;

		//itorate all the vertices and print them out
		cout<<"position of each vertex"<<endl;
		MItMeshVertex meshVertex(originMesh);

		yI.paramsClearAll();
		yI.startGeometry();

		//since MFnMesh does not provide the number of triangles directly, so have to get it in another way
		MItMeshPolygon forCountTriangle(originMesh);
		int numTriangle=0;
		int count;
		for(;!forCountTriangle.isDone();forCountTriangle.next())
		{
			forCountTriangle.numTriangles(count);
			numTriangle=numTriangle+count;
		}

		yI.startTriMesh(yafID,meshFn.numVertices(),numTriangle,false,false,0);
		for(; !meshVertex.isDone();meshVertex.next())
		{
			MPoint locationV=meshVertex.position(MSpace::kObject);
			cout<<"("<<locationV.x<<") ("<<locationV.y<<") ( "<<locationV.z<<")"<<endl;
			yI.addVertex(locationV.x,locationV.y,locationV.z);
			
		}

		//find every triangle face by face
		cout<<"indices of triangle vertice"<<endl;
		MItMeshPolygon meshFace(originMesh);
		for(;!meshFace.isDone();meshFace.next())
		{
			MPointArray points;
			MIntArray pointIndices;
			meshFace.getTriangles(points,pointIndices,MSpace::kObject);
			//get the indices for each vertex of the triangle. the index will be correspond to the point array before
			for(unsigned int i=0;i<pointIndices.length();i++)
			{
				cout<<pointIndices[i]<<"-"<<pointIndices[i+1]<<"-"<<pointIndices[i+2]<<endl;
				yI.addTriangle(pointIndices[i],pointIndices[i+1],pointIndices[i+2],iter->second);
				i=i+2;
			}

		}
		yI.endTriMesh();
		yI.endGeometry();

		selectMesh.next();
	}

	return stat;
}
MStatus getObject::readParticle(yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;
	return stat;
}
