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
#include<iostream>
#include<interface/yafrayinterface.h>
#include<map>
#include<string>
using namespace std;
using namespace yafaray;
MStatus getObject::readObject(yafaray::yafrayInterface_t yI,std::map<string,yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;

	yI.startScene();
	readMesh(yI,materialMap);
	MGlobal::displayInfo("read mesh succeeded!(*^__^*)");
	readParticle(yI,materialMap);
	

	return stat;
}
MStatus getObject::readMesh(yafaray::yafrayInterface_t yI,std::map<string,yafaray::material_t *> &materialMap)
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
				//here the "mat" is till not got
//				yI.addTriangle(pointIndices[i],pointIndices[i+1],pointIndices[i+2],materialMap);
				i=i+2;
			}

		}
		yI.endTriMesh();
		yI.endGeometry();

		selectMesh.next();
	}

	return stat;
}
MStatus getObject::readParticle(yafrayInterface_t yI,std::map<string,yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;
	return stat;
}
