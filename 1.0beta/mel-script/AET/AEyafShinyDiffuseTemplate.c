global proc AEyafShinyDiffuseTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Shiny Diffuse Attribute" -collapse 0;
	        editorTemplate -addControl "Color"; 
	        editorTemplate -callCustom AEShinyColorNew AEShinyReplace "ColorTextureLayer";
	        editorTemplate -addControl "MirrorColor";
	        editorTemplate -callCustom AEShinyMirrorNew AEShinyReplace "MirrorTextureLayer";
			editorTemplate -addControl "DiffuseReflection";
			editorTemplate -addControl "MirrorStrength";
			editorTemplate -addControl "Transparency";
			editorTemplate -addControl "Translucency";
			editorTemplate -addControl "Emit";
			editorTemplate -addControl "Fresnel";
			editorTemplate -addControl "IOR";
			editorTemplate -callCustom AEShinyBumpNew AEShinyReplace "BumpTextureLayer";

		editorTemplate -endLayout;
	
		editorTemplate -addExtraControls;		
	editorTemplate -endScrollLayout;	        
}
global proc AEShinyColorNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Color Texture Layer" layerInput;
	setUITemplate -ppt;
	AEShinyReplace( $shaderName );
}
global proc AEShinyMirrorNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Mirror Texture Layer" layerInput;
	setUITemplate -ppt;
	AEShinyReplace( $shaderName );
}
global proc AEShinyBumpNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Bump Texture Layer" layerInput;
	setUITemplate -ppt;
	AEShinyReplace( $shaderName );
}
global proc AEShinyReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
