global proc AEyafSunLightTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    
	    editorTemplate -beginLayout "YafaRay Sun Light Attribute" -collapse 0;
	    
	        editorTemplate -label "LightColor" -addControl "LightColor";
	        editorTemplate -label "Power" -addControl "Power";
	        editorTemplate -label "Angle" -addControl "Angle";
	        editorTemplate -label "Samples" -addControl "Samples";
	        
	    editorTemplate -endLayout;
	    
	    editorTemplate -beginLayout "Other Attributes" -collapse 1;
	        AElocatorCommon $nodeName;
	    editorTemplate -endLayout;

	    AElocatorInclude $nodeName;
	    editorTemplate -addExtraControls;
	    
	editorTemplate -endScrollLayout;
}
