global proc AEyafGlossyTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Glossy Attribute" -collapse 0;
	        editorTemplate -addControl "DiffuseColor"; 
	        editorTemplate -callCustom AEGlossyDiffuseNew AEGlossyReplace "DiffuseTextureLayer";
	        editorTemplate -addControl "DiffuseReflection";
			editorTemplate -addControl "GlossyColor";
			editorTemplate -callCustom AEGlossyGlossyNew AEGlossyReplace "GlossyTextureLayer";
			editorTemplate -addControl "GlossyReflection";
			editorTemplate -callCustom AEGlossyGlReNew AEGlossyReplace "GlossyReflectionTexLayer";
			editorTemplate -addControl "Exponent";
			editorTemplate -addControl "AsDiffuse";
			editorTemplate -addControl "Anisotropic";
			editorTemplate -addControl "ExponentHorizontal";
			editorTemplate -addControl "ExponentVertical";
			editorTemplate -callCustom AEGlossyBumpNew AEGlossyReplace "BumpTextureLayer";
		editorTemplate -endLayout;

		
		editorTemplate -addExtraControls;
		
	editorTemplate -endScrollLayout;
		
	        
}
global proc AEGlossyDiffuseNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Diffuse Texture Layer" layerInput;
	setUITemplate -ppt;
	AEGlossyReplace( $shaderName );
}
global proc AEGlossyGlossyNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Glossy Texture Layer" layerInput;
	setUITemplate -ppt;
	AEGlossyReplace( $shaderName );
}
global proc AEGlossyGlReNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Glossy Reflection TexLayer" layerInput;
	setUITemplate -ppt;
	AEGlossyReplace( $shaderName );
}
global proc AEGlossyBumpNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Bump Texture Layer" layerInput;
	setUITemplate -ppt;
	AEGlossyReplace( $shaderName );
}
global proc AEGlossyReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
