#ifndef RENDERSETTING
#define RENDERSETTING

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class renderSettingNode : MPxNode
{
public:
	virtual MStatus compute(const MPlug &plug,MDataBlock &data);
	static void * creator();
	static MStatus initialize();
	static const MTypeId id;
private:
	static void setAttribute( void );

	//what is render set?
	//don't add it now

	//general settings
	static MObject renderRayDepth;
	static MObject renderTranspShadow;
	static MObject renderClayRender;
	static MObject renderShadowDepth;
	static MObject renderAutoThreads;
	static MObject renderZBuffer;
	static MObject renderThreads;

	//output settings
	static MObject renderGamma;
	static MObject renderGammaInput;
	static MObject renderClampRGB;
	static MObject renderPremultAlpha;
	static MObject renderOutputMethod;//enum
	static MObject renderTileOrder;//enum
	static MObject renderTileSize;
	static MObject renderAutoSave;
	static MObject renderAlpha;
	static MObject renderShowSampleMask;
	static MObject renderOutputFileType;//enum
	static MObject renderDrawParams;
	//static MObject renderCustomString;//this is text field?

	//AA settings
	static MObject renderAAPasses;
	static MObject renderAASamples;
	static MObject renderAAThreshold;
	static MObject renderAAIncSamples;
	static MObject renderFilterType;//enum
	static MObject renderAAPixelWidth;

    static MObject renderLightType;
	//direct lighting
	static MObject renderDirCaustics;
	static MObject renderPhPhotons;
	static MObject renderPhCaustixMix;
	static MObject renderDirCausticDepth;
	static MObject renderDirCausticRadius;
	static MObject renderDirAO;
	static MObject renderDirAOSamples;
	static MObject renderDirAODist;
	static MObject renderDirAOColor;

	//pathtracing setting
	static MObject renderCausticType;//enum
	//here use direct light's caustic settings
	static MObject renderGIDepth;
	static MObject renderUseBG;
	static MObject renderGIQuality;
	static MObject renderNoRecursive;

	//photon mapping settings
	//depth use pathtracing's
	//phphotons use directlight's
	static MObject renderPhCausPhotons;
	static MObject renderPhDiffuseRad;
	static MObject renderPhCausticRad;
	static MObject renderPhSearch;
	static MObject renderPhCausticMix;//nani?! this shared with directlight too
	static MObject renderPhFG;
	static MObject renderPhFGBounces;
	static MObject renderPhFGSamples;
	static MObject renderPhShowMap;

	//debug settings
	static MObject renderDebugType;//enum
	static MObject renderDebugMaps;

	//an output
	static MObject renderOutput;




};
#endif