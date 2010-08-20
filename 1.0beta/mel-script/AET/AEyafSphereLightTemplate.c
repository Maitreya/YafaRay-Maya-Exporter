global proc AEyafSphereLightTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    
	    editorTemplate -beginLayout "YafaRay Sphere Light Attribute" -collapse 0;
	    
	        editorTemplate -label "LightColor" -addControl "LightColor";
	        editorTemplate -label "Power" -addControl "Power";
	        editorTemplate -label "Radius" -addControl "Radius";
	        editorTemplate -label "MakeGeometry" -addControl "MakeGeometry";
	        editorTemplate -label "Samples" -addControl "Samples";
	        
	    editorTemplate -endLayout;
	    
	    editorTemplate -beginLayout "Other Attributes" -collapse 1;
	        AElocatorCommon $nodeName;
	    editorTemplate -endLayout;

	    AElocatorInclude $nodeName;
	    editorTemplate -addExtraControls;
	editorTemplate -endScrollLayout;
}
