global proc AEyafDisnoiseTextureTemplate(string $nodeName)
{
//	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    editorTemplate -beginLayout "YafaRay Disnoise Texture Attribute" -collapse 0;
	        editorTemplate -addControl "Distort";
			editorTemplate -addControl "DisnoiseSize";
			editorTemplate -addControl "NoiseType1";
			editorTemplate -addControl "NoiseType2";    
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
