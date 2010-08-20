global proc AEyafRoughGlassTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Rough Glass Attribute" -collapse 0;
	        editorTemplate -addControl "AbsorbColor"; 
	        editorTemplate -addControl "AbsorbDistance";
			editorTemplate -addControl "FilterColor";
			editorTemplate -callCustom AERoughFilterNew AERoughReplace "FilterTextureLayer";
			editorTemplate -addControl "MirrorColor";
			editorTemplate -callCustom AERoughMirrorNew AERoughReplace "MirrorTextureLayer";
			editorTemplate -addControl "IOR";
			editorTemplate -addControl "TransmitFilter";
			editorTemplate -addControl "DispersionPower";
			editorTemplate -addControl "FakeShadows";
			editorTemplate -addControl "Exponent";
			editorTemplate -addControl "Roughness";
			editorTemplate -callCustom AERoughBumpNew AERoughReplace "BumpTextureLayer";
		editorTemplate -endLayout;

		
		editorTemplate -addExtraControls;
		
	editorTemplate -endScrollLayout;
		
	        
}
global proc AERoughFilterNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Filter Texture Layer" layerInput;
	setUITemplate -ppt;
	AERoughReplace( $shaderName );
}
global proc AERoughMirrorNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Mirror Texture Layer" layerInput;
	setUITemplate -ppt;
	AERoughReplace( $shaderName );
}
global proc AERoughBumpNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Bump Texture Layer" layerInput;
	setUITemplate -ppt;
	AERoughReplace( $shaderName );
}
global proc AERoughReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
