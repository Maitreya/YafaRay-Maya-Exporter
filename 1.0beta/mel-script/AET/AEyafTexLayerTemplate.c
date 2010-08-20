global proc AEyafTexLayerTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Texture Layer Attribute" -collapse 0;
	        editorTemplate -callCustom AETexInputNew1 AETexInputReplace "LayerInput1";
	        editorTemplate -callCustom AETexInputNew2 AETexInputReplace "LayerInput2";
	        editorTemplate -callCustom AETexInputNew3 AETexInputReplace "LayerInput3";
	        editorTemplate -callCustom AETexInputNew4 AETexInputReplace "LayerInput4";
	    editorTemplate -endLayout;
	    
	    editorTemplate -addExtraControls;
	editorTemplate -endScrollLayout;
}
global proc AETexInputNew1(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Layer Input 1" layerInput;
	setUITemplate -ppt;
	AETexInputReplace( $shaderName );
}
global proc AETexInputNew2(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Layer Input 2" layerInput;
	setUITemplate -ppt;
	AETexInputReplace( $shaderName );
}
global proc AETexInputNew3(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Layer Input 3" layerInput;
	setUITemplate -ppt;
	AETexInputReplace( $shaderName );
}
global proc AETexInputNew4(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -attribute $shaderName -label "Layer Input 4" layerInput;
	setUITemplate -ppt;
	AETexInputReplace( $shaderName );
}
global proc AETexInputReplace(string $shaderName)
{
	setUITemplate -pst attributeEditorTemplate;
	attrNavigationControlGrp -edit -attribute $shaderName layerInput;
	setUITemplate -ppt;
}
