global proc AEyafBlendTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Blend Attribute" -collapse 0;    
	        editorTemplate -callCustom AEBlendNew AEBlendReplace "DiffuseTextureLayer";
	        editorTemplate -addControl "MaterialA"; 
	        editorTemplate -addControl "MaterialB"; 
	        editorTemplate -addControl "BlendValue"; 
		editorTemplate -endLayout;
	
		editorTemplate -addExtraControls;		
	editorTemplate -endScrollLayout;	        
}
global proc AEBlendNew(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Diffuse Texture Layer" layerInput;
	setUITemplate -ppt;
	AEBlendReplace( $shaderName );
}
global proc AEBlendReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
