global proc AEyafCoatedGlossyTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Coated Glossy Attribute" -collapse 0;
	        editorTemplate -addControl "DiffuseColor"; 
	        editorTemplate -callCustom AECoatedDiffuseNew AECoatedReplace "DiffuseTextureLayer";
	        editorTemplate -addControl "DiffuseReflection";
			editorTemplate -addControl "GlossyColor";
			editorTemplate -callCustom AECoatedGlossyNew AECoatedReplace "GlossyTextureLayer";
			editorTemplate -addControl "GlossyReflection";
			editorTemplate -callCustom AECoatedGlReNew AECoatedReplace "GlossyReflectionTexLayer";
			editorTemplate -addControl "Exponent";
			editorTemplate -addControl "AsDiffuse";
			editorTemplate -addControl "Anisotropic";
			editorTemplate -addControl "ExponentHorizontal";
			editorTemplate -addControl "ExponentVertical";
			editorTemplate -addControl "IOR";
			editorTemplate -callCustom AECoatedBumpNew AECoatedReplace "BumpTextureLayer";
		editorTemplate -endLayout;

		
		editorTemplate -addExtraControls;
		
	editorTemplate -endScrollLayout;
		
	        
}
global proc AECoatedDiffuseNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Diffuse Texture Layer" layerInput;
	setUITemplate -ppt;
	AECoatedReplace( $shaderName );
}
global proc AECoatedGlossyNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Glossy Texture Layer" layerInput;
	setUITemplate -ppt;
	AECoatedReplace( $shaderName );
}
global proc AECoatedGlReNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Glossy Reflection TexLayer" layerInput;
	setUITemplate -ppt;
	AECoatedReplace( $shaderName );
}
global proc AECoatedBumpNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Bump Texture Layer" layerInput;
	setUITemplate -ppt;
	AECoatedReplace( $shaderName );
}
global proc AECoatedReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
