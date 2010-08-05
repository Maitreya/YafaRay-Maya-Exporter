#define NOMINMAX
#define _USE_MATH_DEFINES 1
#define PI 3.1415
#include"bReadLight.h"

#include<maya/MGlobal.h>
#include<maya/MSelectionList.h>
#include<maya/MPlug.h>
#include<maya/MPlugArray.h>
#include<maya/MDagPath.h>
#include<maya/MFnDagNode.h>
#include<maya/MMatrix.h>
#include<maya/MFnTransform.h>
#include<maya/MVector.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MFnNumericData.h>
#include <maya/MPoint.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
//#include <maya/M3dView.h>
//#include <maya/MFnCamera.h>
#include<iostream>
#include<interface/yafrayinterface.h>
using namespace yafaray;

MStatus getLight::readLight(yafaray::yafrayInterface_t &yI)
{
	MStatus stat;

	bool hasYafLight=0;

	MString listPoint("ls -type yafPointLight");
	MStringArray pointResult;
	MGlobal::executeCommand(listPoint, pointResult);
	
	if(pointResult.length()>0)
	{
		    hasYafLight=1;
			for(unsigned int i=0;i<pointResult.length();i++)
			{
				//test output
				cout<<pointResult[i].asChar()<<endl;

				MSelectionList list;
				MGlobal::getSelectionListByName(pointResult[i],list);
				for(unsigned int index=0;index<list.length();index++)
				{
					MDagPath pointPath;
					list.getDagPath(index,pointPath);

					MObject pointTransNode=pointPath.transform();
					MFnTransform pointTrans(pointTransNode);
					MGlobal::displayInfo(pointTrans.name());
					MVector transP=pointTrans.getTranslation(MSpace::kTransform);

					//test output
					//this is the position of the point light
					cout<<"============test light==========="<<endl;
					cout<<transP.x<<transP.y<<transP.z<<endl;

					MObject pointDepNode;
					list.getDependNode(index,pointDepNode);
					MFnDependencyNode pointDepFn(pointDepNode);

					yI.paramsClearAll();
					yI.paramsSetString("type","pointlight");

					yI.paramsSetPoint("from",transP.x,transP.y,transP.z);

					MObject pointColor;
					pointDepFn.findPlug("LightColor").getValue(pointColor);
					MFnNumericData pointData(pointColor);
					float pcR,pcG,pcB;
					pointData.getData(pcR,pcG,pcB);
					yI.paramsSetColor("color",pcR,pcG,pcB);

					float pointPower;
					pointDepFn.findPlug("Power").getValue(pointPower);
					yI.paramsSetFloat("power",pointPower);

					cout<<"================point light=============="<<endl;
					cout<<pointDepFn.name().asChar()<<endl;
					yI.createLight(pointDepFn.name().asChar());

				}

			}
	}

	//sphere light
	MString listSphere("ls -type yafSphereLight");
	MStringArray sphereResult;
	MGlobal::executeCommand(listSphere, sphereResult);

	if(sphereResult.length()>0)
	{
		hasYafLight=1;
		for(unsigned int i=0;i<sphereResult.length();i++)
		{
			//test output
			cout<<sphereResult[i].asChar()<<endl;

			MSelectionList list;
			MGlobal::getSelectionListByName(sphereResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MDagPath spherePath;
				list.getDagPath(index,spherePath);

				MObject sphereTransNode=spherePath.transform();
				MFnTransform sphereTrans(sphereTransNode);
				MVector transP=sphereTrans.getTranslation(MSpace::kTransform);


				yI.paramsClearAll();
				yI.paramsSetString("type","spherelight");
				MFnDagNode sphereFn(spherePath);

				yI.paramsSetPoint("from",transP.x, transP.y,transP.z);

				MObject sphereColor;
				sphereFn.findPlug("LightColor").getValue(sphereColor);
				MFnNumericData sphereData(sphereColor);
				float sphereR,sphereG,sphereB;
				sphereData.getData(sphereR,sphereG,sphereB);
				yI.paramsSetColor("color",sphereR,sphereG,sphereB);

				float power;
				sphereFn.findPlug("Power").getValue(power);
				yI.paramsSetFloat("power",power);

				int samples;
				sphereFn.findPlug("Samples").getValue(samples);
				yI.paramsSetInt("samples",samples);

				float radius;
				sphereFn.findPlug("Radius").getValue(radius);
				yI.paramsSetFloat("radius",radius);

				bool makeGeo;
				sphereFn.findPlug("MakeGeometry").getValue(makeGeo);
				if (makeGeo)
				{
					int u=24;
					int v=48;

					int sphereID=makeSphere(yI,u,v,transP.x,transP.y,transP.z,radius);
					yI.paramsSetInt("object",sphereID);
				}

				cout<<"================sphere light=============="<<endl;
				cout<<sphereFn.name().asChar()<<endl;
				yI.createLight(sphereFn.name().asChar());

			}

		}
	}
	MString listSun("ls -type yafSunLight");
	MStringArray sunResult;
	MGlobal::executeCommand(listSun,sunResult);
	if (sunResult.length()>0)
	{
		hasYafLight=1;
		for (unsigned int i=0;i<sunResult.length();i++)
		{
			//test output
			cout<<sunResult[i].asChar()<<endl;

			MSelectionList list;
			MGlobal::getSelectionListByName(sunResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MDagPath sunPath;
				list.getDagPath(index,sunPath);

				MObject sunTransNode=sunPath.transform();
				MFnTransform sunTrans(sunTransNode);
				MTransformationMatrix sunMatrix=sunTrans.transformation();
				MMatrix sMatrix=sunMatrix.asMatrix();
				MVector transP=sunTrans.getTranslation(MSpace::kTransform);

				MPoint direction1(0.0,0.0,0.0);
				MPoint direction2(0.0,0.0,1.0);
				MPoint direction=direction1-direction2;
				direction*=sMatrix;

				yI.paramsClearAll();
				yI.paramsSetString("type","sunlight");
				MFnDagNode sunFn(sunPath);

				yI.paramsSetPoint("from",transP.x, transP.y,transP.z);
				yI.paramsSetPoint("direction",direction.x,direction.y,direction.z);

				MObject sunColor;
				sunFn.findPlug("LightColor").getValue(sunColor);
				MFnNumericData sunData(sunColor);
				float sunR,sunG,sunB;
				sunData.getData(sunR,sunG,sunB);
				yI.paramsSetColor("color",sunR,sunG,sunB);

				float power;
				sunFn.findPlug("Power").getValue(power);
				yI.paramsSetFloat("power",power);

				int samples;
				sunFn.findPlug("Samples").getValue(samples);
				yI.paramsSetInt("samples",samples);

				float angle;
				sunFn.findPlug("Angle").getValue(angle);
				yI.paramsSetFloat("angle",angle);

				cout<<"================sun light=============="<<endl;
				cout<<sunFn.name().asChar()<<endl;
				yI.createLight(sunFn.name().asChar());

			}
		}
	}

	//spot light
	MString listSpot("ls -type yafSpotLight");
	MStringArray spotResult;
	MGlobal::executeCommand(listSpot, spotResult);

	if(spotResult.length()>0)
	{
		hasYafLight=1;
		for(unsigned int i=0;i<spotResult.length();i++)
		{
			//test output
			cout<<spotResult[i].asChar()<<endl;

			MSelectionList list;
			MGlobal::getSelectionListByName(spotResult[i],list);
			for(unsigned int index=0;index<list.length();index++)
			{
				MDagPath spotPath;
				list.getDagPath(index,spotPath);

				MObject spotTransNode=spotPath.transform();
				MFnTransform spotTrans(spotTransNode);
				MTransformationMatrix spotMatrix=spotTrans.transformation();
				MMatrix sMatrix=spotMatrix.asMatrix();
				MVector transP=spotTrans.getTranslation(MSpace::kTransform);

				MPoint direction1(0.0,0.0,0.0);
				MPoint direction2(0.0,-1.0,0.0);
				MPoint direction=direction2-direction1;
				direction*=sMatrix;

				yI.paramsClearAll();
				yI.paramsSetString("type","spotlight");
				MFnDagNode spotFn(spotPath);

				yI.paramsSetPoint("from",transP.x, transP.y,transP.z);
				yI.paramsSetPoint("to",direction.x,direction.y,direction.z);

				MObject spotColor;
				spotFn.findPlug("LightColor").getValue(spotColor);
				MFnNumericData spotData(spotColor);
				float spotR,spotG,spotB;
				spotData.getData(spotR,spotG,spotB);
				yI.paramsSetColor("color",spotR,spotG,spotB);

				float power;
				spotFn.findPlug("Power").getValue(power);
				yI.paramsSetFloat("power",power);

				float coneAngle;
				spotFn.findPlug("ConeAngle").getValue(coneAngle);
				yI.paramsSetFloat("cone_angle",coneAngle);

				float blend;
				spotFn.findPlug("PenumbraDistance").getValue(blend);
				yI.paramsSetFloat("blend",blend);

				bool softShadows;
				spotFn.findPlug("SoftShadows").getValue(softShadows);
				yI.paramsSetBool("soft_shadows",softShadows);

				float fuzzyness;
				spotFn.findPlug("ShadowFuzzyness").getValue(fuzzyness);
				yI.paramsSetFloat("shadowFuzzyness",fuzzyness);

				bool photonOnly;
				spotFn.findPlug("PhotonOnly").getValue(photonOnly);
				yI.paramsSetBool("photon_only",photonOnly);

				int samples;
				spotFn.findPlug("Samples").getValue(samples);
				yI.paramsSetInt("samples",samples);

				cout<<"================spot light=============="<<endl;
				cout<<spotFn.name().asChar()<<endl;
				yI.createLight(spotFn.name().asChar());

			}

		}
	}
	//directional lights
	MString listDirectional("ls -type yafDirectionalLight");
	MStringArray dirResult;
	MGlobal::executeCommand(listDirectional,dirResult);
	if(dirResult.length()>0)
	{
		    hasYafLight=1;

			for (unsigned int i=0; i<dirResult.length(); i++)
			{
				MSelectionList list;
				MGlobal::getSelectionListByName(dirResult[i],list);
				for(unsigned int index=0;index<list.length();index++)
				{
					MDagPath dirShapePath;
					list.getDagPath(index,dirShapePath);
					MFnTransform dirTransform(dirShapePath.transform());
					MTransformationMatrix dirTransMatrix=dirTransform.transformation();
					MMatrix dirMatrix=dirTransMatrix.asMatrix();

					MPoint direction1(0,0,0);
					MPoint direction2(0,0,-1);
					MPoint direction=direction1-direction2;
					direction*=dirMatrix;

					yI.paramsClearAll();
					yI.paramsSetString("type","directional");

					yI.paramsSetPoint("direction",direction.x, direction.y, direction.z);

					MFnDagNode dirShapeFn(dirShapePath);
					MObject dirColor;
					dirShapeFn.findPlug("LightColor").getValue(dirColor);
					MFnNumericData dirData(dirColor);
					float dirR,dirG,dirB;
					dirData.getData(dirR,dirG,dirB);
					yI.paramsSetColor("color",dirR,dirG,dirB);

					float power;
					dirShapeFn.findPlug("Power").getValue(power);
					yI.paramsSetFloat("power",power);

					bool infinite;
					dirShapeFn.findPlug("Infinite").getValue(infinite);
					yI.paramsSetBool("infinite",infinite);

					float radius;
					dirShapeFn.findPlug("Radius").getValue(radius);
					yI.paramsSetFloat("radius",radius);

					MVector dirP=dirTransform.getTranslation(MSpace::kTransform);
					yI.paramsSetPoint("from",dirP.x,dirP.y,dirP.z);

					cout<<"================directional light=============="<<endl;
					cout<<dirShapeFn.name().asChar()<<endl;
					yI.createLight(dirShapeFn.name().asChar());
					
				}
			}
	}

	//area lights
	MString listArea("ls -type yafAreaLight");
	MStringArray areaResult;
	MGlobal::executeCommand(listArea, areaResult);
	if(areaResult.length()>0)
	{
		    hasYafLight=1;

			for(unsigned int i=0;i<areaResult.length();i++)
			{
				MSelectionList list;
				MGlobal::getSelectionListByName(areaResult[i],list);
				for(unsigned int index=0;index<list.length();index++)
				{
					MDagPath areaShapePath;
					list.getDagPath(index,areaShapePath);
					//areaTransform is the transform node of this light, all the location information is here
					MFnTransform areaTransform(areaShapePath.transform());
					MTransformationMatrix areaTransMatrix=areaTransform.transformation();
					MMatrix areaMatrix=areaTransMatrix.asMatrix();
					//this is the shape node of the light, all the attribute is here

					MPoint corner(-1.0,-1.0,0);
					MPoint point1(-1.0,1.0,0);
					MPoint point2(1.0,-1.0,0);
					corner*=areaMatrix;
					point1*=areaMatrix;
					point2*=areaMatrix;

					yI.paramsClearAll();
					yI.paramsSetString("type","arealight");

					cout<<"===============area light=============="<<endl;
					MFnDagNode areaShapeFn(areaShapePath);
					int samples;
					areaShapeFn.findPlug("Samples").getValue(samples);
		//			cout<<"samples"<<samples<<endl;
					yI.paramsSetInt("samples",samples);

					yI.paramsSetPoint("corner",corner.x, corner.y, corner.z);
		//			cout<<"corner"<<corner.x<<corner.y<<corner.z<<endl;
					yI.paramsSetPoint("point1",point1.x, point1.y, point1.z);
		//			cout<<"point1"<<point1.x<<point1.y<<point1.z<<endl;
					yI.paramsSetPoint("point2",point2.x, point2.y, point2.z);
		//			cout<<"point2"<<point2.x<<point2.y<<point2.z<<endl;

					MVector areaP=areaTransform.getTranslation(MSpace::kTransform);
		//			cout<<"from"<<areaP.x<<areaP.y<<areaP.z<<endl;
					yI.paramsSetPoint("from",areaP.x, areaP.y, areaP.z);

					
					MObject areaColor;
					areaShapeFn.findPlug("LightColor").getValue(areaColor);
					MFnNumericData areaData(areaColor);
					float areaR,areaG,areaB;
					areaData.getData(areaR,areaG,areaB);
		//			cout<<"color"<<areaR<<areaG<<areaB<<endl;
					yI.paramsSetColor("color",areaR,areaG,areaB);

					float power;
					areaShapeFn.findPlug("Power").getValue(power);
		//			cout<<"power"<<power<<endl;
					yI.paramsSetFloat("power",power);

					cout<<areaShapeFn.name().asChar()<<endl;
					yI.createLight(areaShapeFn.name().asChar());

				}
			}
	}
	//if there is no light in the scene, create a default one
	//but this one will not be available if users create new ones
	if (hasYafLight==false)
	{
		yI.paramsClearAll();
		yI.paramsSetString("type","directional");
		//use camera direction and position so that default light always can be seen no matter where user watch from 
		//M3dView currentView=M3dView::active3dView();
		//MDagPath cameraPath;
		//MFnCamera currentCam(cameraPath);
		//MVector lightPosition=currentCam.eyePoint(MSpace::kWorld);
		//MVector lightDirection=currentCam.viewDirection(MSpace::kWorld);

		//currentView.getCamera(cameraPath);
		yI.paramsSetPoint("direction",0.0,0.0,1.0);
		yI.paramsSetPoint("from",0.0,0.0,0.0);
		yI.paramsSetColor("color",0.8f,0.8f,0.8f);
		yI.paramsSetFloat("power",1.0f);
		yI.paramsSetBool("infinite",true);
		yI.paramsSetFloat("radius",1.0f);
		yI.createLight("yafDefaultLight1");

		yI.paramsClearAll();
		yI.paramsSetString("type","directional");
		yI.paramsSetPoint("direction",1.0,0.0,-1.0);
		yI.paramsSetPoint("from",0.0,0.0,0.0);
		yI.paramsSetColor("color",0.8f,0.8f,0.8f);
		yI.paramsSetFloat("power",1.0f);
		yI.paramsSetBool("infinite",true);
		yI.paramsSetFloat("radius",1.0f);
		yI.createLight("yafDefaultLight2");

		MGlobal::displayWarning("(yafaray) you haven't created your own light yet!");
	}

	return stat;
}
int getLight::makeSphere(yafaray::yafrayInterface_t &yI, int u, int v, double x, double y, double z, float radius)
{
	int sphereID=yI.getNextFreeID();
	yI.startGeometry();
	yI.startTriMesh(sphereID,2+(u-1)*v,2*(u-1)*v,false,false);
	yI.addVertex(x,y,z+radius);
	yI.addVertex(x,y,z-radius);
	for (int i=1;i<v;i++)
	{
		float t=i/float(v);
		double sinV=sin(2.0*PI*t);
		double cosV=cos(2.0*PI*t);
		for (int j=2;j<u;j++)
		{
			float s=j/float(u);
			double sinU=sin(PI*s);
			double cosU=cos(PI*s);
			yI.addVertex(x+cosV*cosU*radius,y+sinV*sinU*radius,z+cosU*radius);
		}
	}
	for (int i=1;i<v;i++)
	{
		yI.addTriangle(0,2+i*(u-1),2+((i+1)%v)*(u-1),NULL);
		yI.addTriangle(1,((i+1)%v)*(u-1)+u, i*(u-1)+u,NULL);
		for (int j=1;j<u-2;j++)
		{
			yI.addTriangle( 2+i*(u-1)+j, 2+i*(u-1)+j+1, 2+((i+1)%v)*(u-1)+j,NULL );
			yI.addTriangle( 2+i*(u-1)+j+1, 2+((i+1)%v)*(u-1)+j+1, 2+((i+1)%v)*(u-1)+j, NULL );
		}
	}
	yI.endTriMesh();
	yI.endGeometry();
	return sphereID;
}