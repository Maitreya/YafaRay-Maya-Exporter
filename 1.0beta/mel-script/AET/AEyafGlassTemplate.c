global proc AEyafGlassTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Glass Attribute" -collapse 0;
	        editorTemplate -addControl "AbsorbColor"; 
	        editorTemplate -addControl "AbsorbDistance";
			editorTemplate -addControl "FilterColor";
			editorTemplate -callCustom AEGlassFilterNew AEGlassReplace "FilterTextureLayer";
			editorTemplate -addControl "MirrorColor";
			editorTemplate -callCustom AEGlassMirrorNew AEGlassReplace "MirrorTextureLayer";
			editorTemplate -addControl "IOR";
			editorTemplate -addControl "TransmitFilter";
			editorTemplate -addControl "DispersionPower";
			editorTemplate -addControl "FakeShadows";
			editorTemplate -callCustom AEGlassBumpNew AEGlassReplace "BumpTextureLayer";
		editorTemplate -endLayout;

		
		editorTemplate -addExtraControls;
		
	editorTemplate -endScrollLayout;
		
	        
}
global proc AEGlassFilterNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Filter Texture Layer" layerInput;
	setUITemplate -ppt;
	AEGlassReplace( $shaderName );
}
global proc AEGlassMirrorNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Mirror Texture Layer" layerInput;
	setUITemplate -ppt;
	AEGlassReplace( $shaderName );
}
global proc AEGlassBumpNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Bump Texture Layer" layerInput;
	setUITemplate -ppt;
	AEGlassReplace( $shaderName );
}
global proc AEGlassReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
