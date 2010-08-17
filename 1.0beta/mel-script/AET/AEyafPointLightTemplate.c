global proc AEyafPointLightTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    
	    editorTemplate -beginLayout "YafaRay Point Light Attribute" -collapes 0;
	        editorTemplate -label "LightColor" -addControl "LightColor";
	        editorTemplate -label "Power" -addControl "Power";
	    editorTemplate -endLayout;
	editorTemplate -endScrollLayout;
}
