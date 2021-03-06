global proc AEyafImageTextureTemplate(string $nodeName)
{
//	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Image Texture Attribute" -collapse 0;
	        editorTemplate -callCustom AEImageNew AEImageChange "ImageFile";
	        separator;
	        editorTemplate -label "Repeat X" -addControl "RepeatX"; 
	        editorTemplate -label "Repeat Y" -addControl "RepeatY"; 
	        editorTemplate -addControl "UseAlpha";
			editorTemplate -addControl "CalculateAlpha";
			editorTemplate -addControl "NormalMap"; 
			editorTemplate -addControl "MinX";
			editorTemplate -addControl "MinY";
			editorTemplate -addControl "MaxX";
			editorTemplate -addControl "MaxY";
		editorTemplate -endLayout;
		
		editorTemplate -beginLayout "YafaRay Texture Layer Attribute" -collapse 0;
		    editorTemplate -addControl "MixMethod";
		    editorTemplate -addControl "TextureColor";
		    editorTemplate -addControl "TextureColorWeight";
		    editorTemplate -addControl "DefValue";
		    editorTemplate -addControl "ValueWeight";
		    editorTemplate -addControl "DoColor";
		    editorTemplate -addControl "Negative";
		    editorTemplate -addControl "NoRGB";
		    editorTemplate -addControl "Stencil";
		editorTemplate -endLayout;
		
		editorTemplate -beginLayout "YafaRay Mapping Method" -collapse 0;
		    editorTemplate -addControl "MappingMethod";
		editorTemplate -endLayout;
		editorTemplate -beginLayout "YafaRay Texture Coordinate" -collapse 0;
		    editorTemplate -addControl "TextureCoordinate";
		editorTemplate -endLayout;
		
		editorTemplate -addExtraControls;
		
	editorTemplate -endScrollLayout;
		
	        
}

global proc AEImageNew(string $imageName)
{
	setUITemplate -pushTemplate attributeEditorTemplate;
	rowLayout -numberOfColumns 3 imageFileNameLayout;
	    text -label "Image File";
	    textField imageNameField;
	    symbolButton -image "yafBrowse.png" imageBrowse;
	    setParent ..;
	setUITemplate -popTemplate;
	AEImageChange($imageName);
}

global proc AEImageChange(string $imageName)
{
	connectControl -fileName imageNameField $imageName;
	button -e -c ("AEImageBrowse \""+$imageName+"\"" ) imageBrowse;

}
global proc AEImageBrowse(string $imageName)
{
	string $file=`fileDialog -mode 0`;
	setAttr $imageName -type "string" $file;
}
