#ifndef WORLDSETTING
#define WORLDSETTING

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

//the declearation of the world setting node
class worldSettingNode : MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;

private:
	static void setAttribute( void);
//all the attribute of world setting node

	//enumeric attribute
	static MObject backgroundTypes;
	static MObject volumeInitTypes;
	static MObject DSSkyColorSpaces;

	//single color attribute
	static MObject backgroundColor;

	//gradient color attribute
	static MObject horizonColor;
	static MObject zenithColor;
	static MObject horGroundColor;
	static MObject zenGroundColor;

	//texture attribute
	static MObject texRotation;

	//sunsky attribute
	static MObject turbidity;
	static MObject AHorBrght;
	static MObject BHorSprd;
	static MObject CSunBrght;
	static MObject DSunSize;
	static MObject EBacklight;
	//three froms are not attributes, they should be read from the selected light
	//the preview of the sunlight direction can't be draw now, does it using QT?
	//x,y,x are shared with DSSky
	static MObject xDirection;
	static MObject yDirection;
	static MObject zDirection;
	static MObject realSun;
	static MObject sunPower;
	static MObject skyLight;
	static MObject skySamples;

	//DarkTide's sunsky attribute
	static MObject DSTurbidity;
	static MObject renderDSA;
	static MObject renderDSB;
	static MObject renderDSC;
	static MObject renderDSD;
	static MObject renderDSE;
	//sun direction shared with sunsky
	static MObject DSAltitude;
	static MObject DSNight;
	static MObject DSRealSun;
	static MObject DSSunPower;
	static MObject DSSkyLight;
	static MObject DSSkyPower;
	static MObject DSSkySamples;
	static MObject DSSkyBright;
	static MObject DSExposure;
	static MObject DSGammaEncoding;

	//these  appear in each type of background
	static MObject useIBL;
	static MObject IBLSamples;
	static MObject backgroundDiffuse;
	static MObject backgoundCaustic;
	static MObject backgoundPower;

	//volume settings
//	static MObject volumeIntType;//enum
	static MObject volumeStepSize;
	static MObject volumeAdaptive;
	static MObject volumeOptimize;
	static MObject volumeAttMapScale;
    static MObject volumeSkyST;
	static MObject volumeAlpha;


	//the node needs an output, even we dont need it...
	static MObject worldOutput;






};
#endif