#define NOMINMAX
#define _USE_MATH_DEFINES 1
#include "bReadObject.h"

#include<maya/MGlobal.h>
#include<maya/MSelectionList.h>
#include<maya/MString.h>
#include <maya/MStringArray.h>
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
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
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
	readParticle(yI,materialMap);
	

	return stat;
}
MStatus getObject::readMesh(yafaray::yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;

	//select all the mesh in the scene
	MString readMeshCmd("ls -type mesh");
	MStringArray readMeshResult;
	MGlobal::executeCommand(readMeshCmd, readMeshResult);

	for(unsigned int i=0; i<readMeshResult.length(); i++)
	{
			MSelectionList meshList;
			MGlobal::getSelectionListByName(readMeshResult[i],meshList);

			for(unsigned int index=0; index<meshList.length(); index++)
			{
				MDagPath meshPath;
				meshList.getDagPath(index, meshPath);

				//this is for getting the number of faces of the mesh
				MFnMesh meshFn(meshPath);
				cout<<"==============mesh test=============="<<endl;
				cout<<"mesh name:"<<meshFn.name()<<endl;
				
				//find the material of the mesh
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
				cout<<"material name:"<<sName<<endl;

				std::map<string , yafaray::material_t *>::iterator iter=materialMap.find(sName);
				if(iter==materialMap.end())
				{
					//if searched in the map and didn't find the shader name of this object(means this object didn't use yafaray's material)
					//we should give it an default shader
					//to implement this, should export a default shader first, in the read shader step
					//iter=materialMap.find("yafDefaultMaterial");

					//using method above got some problem...when two shape use one trasform node
					//found this problem when using ncloth
					//so still jump the ones who didn't use yafaray's material
					cout<<"this mesh doesn't use yafaray's material"<<endl;
					MGlobal::displayInfo("(yafaray mesh) this mesh doesn't use yafaray's material!");
					break;
				}

				unsigned int yafID=yI.getNextFreeID();

				//itorate all the vertices and print them out
				MItMeshVertex meshVertex(meshPath);

				yI.paramsClearAll();
				yI.startGeometry();

				//since MFnMesh does not provide the number of triangles directly, so have to get it in another way
				MItMeshPolygon forCountTriangle(meshPath);
				int numTriangle=0;
				int count;
				for(;!forCountTriangle.isDone();forCountTriangle.next())
				{
					forCountTriangle.numTriangles(count);
					numTriangle=numTriangle+count;
				}

				yI.startTriMesh(yafID, meshFn.numVertices(), numTriangle, false, false, 0);
				for(; !meshVertex.isDone();meshVertex.next())
				{
					MPoint locationV=meshVertex.position(MSpace::kWorld);
					yI.addVertex(locationV.x,locationV.y,locationV.z);
					
				}

				//find every triangle face by face
				MItMeshPolygon meshFace(meshPath);
				for(;!meshFace.isDone();meshFace.next())
				{
					MPointArray points;
					MIntArray pointIndices;
					float UV0[2];
					float UV1[2];
					float UV2[2];
					meshFace.getTriangles(points,pointIndices,MSpace::kWorld);
					//get the indices for each vertex of the triangle. the index will be correspond to the point array before
					for(unsigned int i=0;i<pointIndices.length();i++)
					{
						meshFace.getUVAtPoint(points[i],UV0,MSpace::kObject);
						meshFace.getUVAtPoint(points[i+1],UV1,MSpace::kObject);
						meshFace.getUVAtPoint(points[i+2],UV2,MSpace::kObject);

						////test output
						//cout<<"================="<<i<<"================"<<endl;
						//cout<<UV0[0]<<UV0[1]<<endl;
						//cout<<UV1[0]<<UV1[1]<<endl;
						//cout<<UV2[0]<<UV2[1]<<endl;
						int uvIndex0=yI.addUV(UV0[0],UV0[1]);
						int uvIndex1=yI.addUV(UV1[0],UV1[1]);
						int uvIndex2=yI.addUV(UV2[0],UV2[1]);
						yI.addTriangle(pointIndices[i],pointIndices[i+1],pointIndices[i+2],uvIndex0,uvIndex1,uvIndex2,iter->second);
						//yI.addTriangle(pointIndices[i],pointIndices[i+1],pointIndices[i+2],iter->second);
						i=i+2;
					}

				}
				yI.endTriMesh();
				yI.smoothMesh(0, 181);
				yI.endGeometry();

			}

	}
	return stat;
}
MStatus getObject::readParticle(yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap)
{
	MStatus stat=MStatus::kSuccess;
	return stat;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/**********************here are the code that with the test output***********************/
////////////////////////////////////////////////////////////////////////////////////////////////

//MStatus getObject::readMesh(yafaray::yafrayInterface_t &yI,std::map<string , yafaray::material_t *> &materialMap)
//{
//	MStatus stat=MStatus::kSuccess;
//
//	//select all the mesh in the scene
//	MSelectionList selectList;
//	MString matchPolygon("*");
//	MGlobal::getSelectionListByName(matchPolygon,selectList);
//	MItSelectionList selectMesh(selectList,MFn::kMesh);
//
//	//this itorate should be done on only visiable meshes, but has not implement yet
//	//how to find the visiability attribute of a mesh?
//	for(; !selectMesh.isDone();selectMesh.next())
//	{
//		MDagPath meshPath;
//		selectMesh.getDagPath(meshPath);
//
//		MFnDagNode meshDagFn(meshPath);
//		cout<<"the mesh name is "<<meshDagFn.name().asChar()<<endl;
//
//		//this is for getting the number of faces of the mesh
//		MFnMesh meshFn(meshPath);
//
//		//find the material of the mesh
//		MObjectArray sgArray;
//		MIntArray indexSG;
//		meshFn.getConnectedShaders(0, sgArray, indexSG);
//		MFnDependencyNode sgFn(sgArray[0]);
//		MPlug surfacePlug=sgFn.findPlug("surfaceShader");
//		MPlugArray sourceArray;
//		MString shaderName;
//		if (surfacePlug.connectedTo(sourceArray, true, false))
//		{
//			MPlug sourcePlug=sourceArray[0];
//			MObject sourceNode=sourcePlug.node();
//			MFnDependencyNode sourceFn(sourceNode);
//			shaderName=sourceFn.name();
//		}
//
//		cout<<"+++++++++++++++++++mat test++++++++++++++++++"<<endl;
//		string sName(shaderName.asChar());
//		if (sName!="yafGlass1")
//		{
//			cout<<sName<<endl;
//		}
//		cout<<"+++++++++++++++++++mat test++++++++++++++++++"<<endl;
//
//		std::map<string , yafaray::material_t *>::iterator iter=materialMap.find(sName);
//		if(iter==materialMap.end())
//		{
//			//here just a temp way
//			//actually, if searched in the map and didn't find the shader name of this object(means this object didn't use yafaray's shader)
//			//we should give it an default shader
//			//to implement this, should export a default shader first, in the read shader step
//			//implement this later
//			MGlobal::displayError("iterator is useless");
//			return MStatus::kFailure;
//		}
//
//		unsigned int yafID=yI.getNextFreeID();
//		cout<<"the mesh id is "<<yafID<<endl;
//
//		//itorate all the vertices and print them out
//		cout<<"position of each vertex"<<endl;
//		MItMeshVertex meshVertex(meshPath);
//
//		yI.paramsClearAll();
//		yI.startGeometry();
//
//		//since MFnMesh does not provide the number of triangles directly, so have to get it in another way
//		MItMeshPolygon forCountTriangle(meshPath);
//		int numTriangle=0;
//		int count;
//		for(;!forCountTriangle.isDone();forCountTriangle.next())
//		{
//			forCountTriangle.numTriangles(count);
//			numTriangle=numTriangle+count;
//		}
//
//		yI.startTriMesh(yafID, meshFn.numVertices(), numTriangle, false, false, 0);
//		for(; !meshVertex.isDone();meshVertex.next())
//		{
//			MPoint locationV=meshVertex.position(MSpace::kWorld);
//			cout<<"("<<locationV.x<<") ("<<locationV.y<<") ( "<<locationV.z<<")"<<endl;
//			yI.addVertex(locationV.x,locationV.y,locationV.z);
//
//		}
//
//		//find every triangle face by face
//		cout<<"indices of triangle vertice"<<endl;
//		MItMeshPolygon meshFace(meshPath);
//		for(;!meshFace.isDone();meshFace.next())
//		{
//			MPointArray points;
//			MIntArray pointIndices;
//			meshFace.getTriangles(points,pointIndices,MSpace::kWorld);
//			//get the indices for each vertex of the triangle. the index will be correspond to the point array before
//			for(unsigned int i=0;i<pointIndices.length();i++)
//			{
//				cout<<pointIndices[i]<<"-"<<pointIndices[i+1]<<"-"<<pointIndices[i+2]<<endl;
//				yI.addTriangle(pointIndices[i],pointIndices[i+1],pointIndices[i+2],iter->second);
//				i=i+2;
//			}
//
//		}
//		yI.endTriMesh();
//		yI.smoothMesh(0, 181);
//		yI.endGeometry();
//
//		selectMesh.next();
//	}
//
//	return stat;
//}