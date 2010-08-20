global proc AEyafSpotLightTemplate(string $nodeName)
{
	AEswatchDisplay $nodeName;
	editorTemplate -beginScrollLayout;
	    
	    editorTemplate -beginLayout "YafaRay Spot Light Attribute" -collapse 0;
	        editorTemplate -label "LightColor" -addControl "LightColor";
	        editorTemplate -label "Power" -addControl "Power";
	        editorTemplate -label "ConeAngle" -addControl "ConeAngle";
	        editorTemplate -label "PenumbraDistance" -addControl "PenumbraDistance";
	        editorTemplate -label "SoftShadows" -addControl "SoftShadows";
	        editorTemplate -label "ShadowFuzzyness" -addControl "ShadowFuzzyness";
	        editorTemplate -label "PhotonOnly" -addControl "PhotonOnly";
	        editorTemplate -label "Samples" -addControl "Samples";
	        
	    editorTemplate -endLayout;
	    
	    editorTemplate -beginLayout "Other Attributes" -collapse 1;
	        AElocatorCommon $nodeName;
	    editorTemplate -endLayout;

	    AElocatorInclude $nodeName;
	    editorTemplate -addExtraControls;
	    
	editorTemplate -endScrollLayout;
}
