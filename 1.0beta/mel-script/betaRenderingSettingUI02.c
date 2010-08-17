global proc ySettingUI()
{
	yafLoad;
	
    global string $yafRenderSettingWindow;
    if(`windowPref -exists $yafRenderSettingWindow`) {windowPref -remove $yafRenderSettingWindow;}
    if(`window -exists $yafRenderSettingWindow`) {deleteUI $yafRenderSettingWindow;}
    
    string $nodeTemp[]=`ls -type "yafRenderSetting"`;
	string $yafRenderSettingNode;
	if(size($nodeTemp)==0)
	{
		$yafRenderSettingNode=`createNode yafRenderSetting`;
		select -deselect $yafRenderSettingNode;
	}
    else
    {
		$yafRenderSettingNode=$nodeTemp[0];
	}

    $yafRenderSettingWindow=`window -title "YafaRay Setting Window" -width 435 -height 600`;
        string $yafLogo=`columnLayout -parent $yafRenderSettingWindow`;
          image -parent $yafLogo -image "yaflogo.png";
        string $yafRenderCmd=`columnLayout -parent $yafRenderSettingWindow`;
            button -parent $yafRenderCmd -label "R-----E-----N-----D-----E-----R" -command "yafRender";
            button -parent $yafRenderCmd -label "Preview Render" -command "yafRenderPreview";

        string $yafSetting = `tabLayout -parent $yafRenderSettingWindow -width 430 /*-height 500*/ -innerMarginWidth 5 -innerMarginHeight 5`;

		  string $yafObjSetting = `columnLayout -parent $yafSetting`;
		   yObjectSettingUI($yafObjSetting);     

          string $yafCameraSetting = `columnLayout -parent $yafSetting`;
            yCameraSettingUI($yafCameraSetting);
                       
          string $yafWorldSetting = `columnLayout -parent $yafSetting`;
            yWorldSettingUI($yafWorldSetting);

          string $yafIntegratorSetting = `columnLayout -parent $yafSetting`;
            yIntegratorSettingUI($yafIntegratorSetting,$yafRenderSettingNode);
            //setParent ..;
          
          string $yafRenderSetting = `columnLayout -parent $yafSetting`;
            yRenderSettingUI($yafRenderSetting,$yafRenderSettingNode);
          
          string $yafOutputSetting = `columnLayout -parent $yafSetting`;
            yOutputSettingUI($yafOutputSetting,$yafRenderSettingNode);


        tabLayout -edit -tabLabel $yafObjSetting "Object"
                        -tabLabel $yafCameraSetting "Camera"
                        -tabLabel $yafWorldSetting "World"
                        -tabLabel $yafIntegratorSetting "Integrator"
                        -tabLabel $yafRenderSetting "Render"
                        -tabLabel $yafOutputSetting "Output"
                        $yafSetting;
    showWindow;
    

}
global proc yObjectSettingUI(string $objSettingTab)
{
	      string $yafObjScroll = `scrollLayout -parent $objSettingTab -width 420 -height 550`;
            separator -parent $yafObjScroll -height 10 -style "none";
            text -parent $yafObjScroll -label "     this menu is for setting meshlight or volumn for mesh objects";
            separator -parent $yafObjScroll -height 10 -style "none";
            
            string $yafMeshLight =` frameLayout -parent $yafObjScroll -label "Meshlight" -width 390 -collapsable 1`;
              string $yafMeshLightEditor = `columnLayout -parent $yafMeshLight -adj 1`;
                string $yafMeshCheckbox = `checkBox -parent $yafMeshLightEditor -label "enable meshlight" -value false `;
				        // -onCommand "columnLayout -edit -enable true $yafMeshLightDetailEditor"
						 //-offCommand "columnLayout -edit -enable false $yafMeshLightDetailEditor";
			  separator -parent $yafMeshLight -height 15 -style "in";
              string $yafMeshLightDetailEditor = `columnLayout -parent $yafMeshLight -enable false`;
                colorSliderGrp -parent $yafMeshLightDetailEditor -label "Meshlight Color" -rgb 1 1 1;
                floatSliderGrp -parent $yafMeshLightDetailEditor -label "Power" -minValue 0.0 -maxValue 10000.0 -field true;
                checkBoxGrp -parent $yafMeshLightDetailEditor -label "" -label1 "Double Sided" -numberOfCheckBoxes 1;
                intSliderGrp -parent $yafMeshLightDetailEditor -label "Samples" -minValue 0 -maxValue 512 -field true;
              separator -parent $yafMeshLight -height 15 -style "in";
                //setParent..;
              //setParent ..;
            string $yafBGP = `frameLayout -parent $yafObjScroll -label "BGPLight" -width 390 -collapsable 1`;
              string $yafBGPEditor = `columnLayout -parent $yafBGP -adj 1`;
                string $yafBGPCheckbox = `checkBox -parent $yafBGPEditor -label "enable BG Portal Light" -value false`;
				         //-onCommand ("columnLayout -edit -enable true "+$yafBGPDetailEditor)
						 //-offCommand ("columnLayout -edit -enable false "+$yafBGPDetailEditor);
                
              separator -parent $yafBGP -height 15 -style "in";
              string $yafBGPDetailEditor = `columnLayout -parent $yafBGP -enable false`;
                floatSliderGrp -parent $yafBGPDetailEditor -label "Power" -minValue 0.0 -maxValue 10000.0 -field true;
                intSliderGrp -parent $yafBGPDetailEditor -label "Samples" -minValue 0 -maxValue 512 -field true;
                checkBoxGrp -parent $yafBGPDetailEditor -label "" -label1 "Diffuse Photons" -numberOfCheckBoxes 1;
                checkBoxGrp -parent $yafBGPDetailEditor -label "" -label1 "Caustic Photons" -numberOfCheckBoxes 1;
                checkBoxGrp -parent $yafBGPDetailEditor -label "" -label1 "Photon Only" -numberOfCheckBoxes 1;
              separator -parent $yafBGP -height 15 -style "in";

              
			string $yafVolume = `frameLayout -parent $yafObjScroll -label "Volume" -width 390 -collapsable 1`;
			  string $yafVolumeEditor = `columnLayout -parent $yafVolume -adj 1`;
			    string $yafVolumeCheckbox=`checkBox -parent $yafVolumeEditor -label "enable Volume" -value false`;
				         //-onCommand "columnLayout -edit -enable true $yafVolumeDetailEditor"
						 //-offCommand "columnLayout -edit -enable false $yafVolumeDetailEditor";
			  separator -parent $yafVolume -height 15 -style "in";
			  string $yafVolumeDetailEditor = `columnLayout -parent $yafVolume -enable false`;
			    attrEnumOptionMenuGrp -parent $yafVolumeDetailEditor -label "Volume Region" //here an attribute should be added
			                          -enumeratedItem 0 "Uniform Volume"
			                          -enumeratedItem 1 "ExpDensity Volume"
			                          -enumeratedItem 2 "Noise Volume";
			    floatSliderGrp -parent $yafVolumeDetailEditor -label "Absoption" -minValue 0.0 -maxValue 1.0 -field true;
			    floatSliderGrp -parent $yafVolumeDetailEditor -label "Scatter" -minValue 0.0 -maxValue 1.0 -field true;
			    string $yafExpdensityVolume = `frameLayout -parent $yafVolumeDetailEditor -label "ExpDensity Volume" -width 385 -enable false`;
			      floatSliderGrp -parent $yafExpdensityVolume -label "Height" -minValue 0.0 -maxValue 1000.0 -field true;
			      floatSliderGrp -parent $yafExpdensityVolume -label "Steepness" -minValue 0.0 -maxValue 10.0 -field true;
			    string $yafNoiseVolume = `frameLayout -parent $yafVolumeDetailEditor -label "Noise Volume" -width 385 -enable false`;
			      floatSliderGrp -parent $yafNoiseVolume -label "Sharpness" -minValue 1.0 -maxValue 100.0 -field true;
			      floatSliderGrp -parent $yafNoiseVolume -label "Cover" -minValue 0.0 -maxValue 1.0 -field true;
			      floatSliderGrp -parent $yafNoiseVolume -label "Density" -minValue 0.1 -maxValue 100.0 -field true;
			  separator -parent $yafVolume -height 15 -style "in";
		  checkBox -edit 
		           -onCommand ("columnLayout -edit -enable true " +$yafMeshLightDetailEditor)
		           -offCommand ("columnLayout -edit -enable false " +$yafMeshLightDetailEditor)
				   $yafMeshCheckbox;
		  checkBox -edit 
		           -onCommand ("columnLayout -edit -enable true "+$yafBGPDetailEditor)
				   -offCommand ("columnLayout -edit -enable false "+$yafBGPDetailEditor)
				   $yafBGPCheckbox;
		  checkBox -edit 
		           -onCommand ("columnLayout -edit -enable true "+$yafVolumeDetailEditor)
				   -offCommand ("columnLayout -edit -enable false "+$yafVolumeDetailEditor)
				   $yafVolumeCheckbox;
}
global proc yCameraSettingUI(string $camSettingTab)
{
	        string $yafCameraScroll = `scrollLayout -parent $camSettingTab -width 420 -height 550`;
              separator -parent $yafCameraScroll -height 10 -style "none";
              text -parent $yafCameraScroll -label "     this menu is for create and edit cameras";
              separator -parent $yafCameraScroll -height 10 -style "none";
              string $yafPerspective = `frameLayout -parent $yafCameraScroll -label "Perspective Camera" -width 390 -collapsable 1`;
                string $yafPerspCreateButton = `columnLayout -parent $yafPerspective`;
                  string $yafPerspButton=`button -parent $yafPerspCreateButton -label "Create Perspective Camera" -width 200`;
				         //-command "columnLayout -edit -enable true $yafPerspDetail";
                separator -parent $yafPerspective -height 15 -style "in";
                string $yafPerspDetail = `columnLayout -parent $yafPerspective -enable false`;
                  attrEnumOptionMenuGrp -parent $yafPerspDetail -label "Bokeh Type" //here needed an attribute
                                        -enumeratedItem 0 "disk1"
                                        -enumeratedItem 1 "disk2"
                                        -enumeratedItem 2 "triangle"
                                        -enumeratedItem 3 "square"
                                        -enumeratedItem 4 "pentagon"
                                        -enumeratedItem 5 "hexagon"
                                        -enumeratedItem 6 "ring";
				  floatSliderGrp -parent $yafPerspDetail -label "Rotation" -minValue 0.0 -maxValue 180.0 -field true;
				  intSliderGrp -parent $yafPerspDetail -label "Aperture Size" -minValue 0 -maxValue 20 -field true;
				  attrEnumOptionMenuGrp -parent $yafPerspDetail -label "Bokeh Bias"
				                        -enumeratedItem 0 "uniform"
										-enumeratedItem 1 "edge"
										-enumeratedItem 2 "center";
				  //separator -parent $yafPerspDetail -height 15 -style "in";
				  floatSliderGrp -parent $yafPerspDetail -label "Dof Distance" -minValue 0.0 -maxValue 1000.0 -field true;
				  string $yafFocusCheckbox=`checkBoxGrp -parent $yafPerspDetail -numberOfCheckBoxes 1 -label "" -label1 "Object Focus"`;
				              //-onCommand1 "textFieldGrp -edit -enable true $yafPerspText"
							  //-offCommand1 "textFieldGrp -edit -enable false $yafPerspText";
				  string $yafPerspText = `textFieldGrp -parent $yafPerspDetail -label "Dof Object" -editable true -enable false`;                                    
                separator -parent $yafPerspective -height 15 -style "in";
                
              string $yafOrthographic = `frameLayout -parent $yafCameraScroll -label "Orthographic Camera" -width 390 -collapsable 1`;
                string $yafOrthoCreateButton = `columnLayout -parent $yafOrthographic`;
                  string $yafOrthoButton=`button -parent $yafOrthoCreateButton -label "Create Orthographic Camera" -width 200`;
				         //-command "columnLayout -edit -enable true $yafOrthoDetail";
                separator -parent $yafOrthographic -height 15 -style "in";
                string $yafOrthoDetail = `columnLayout -parent $yafOrthographic -enable false`;
                   intSliderGrp -parent $yafOrthoDetail -label "Scale" -minValue 0 -maxValue 10000 -field true;
                separator -parent $yafOrthographic -height 15 -style "in";
                
              string $yafAngular = `frameLayout -parent $yafCameraScroll -label "Angular Camera" -width 390 -collapsable 1`;
                string $yafAngularCreateButton = `columnLayout -parent $yafAngular`;
                  string $yafAngularButton=`button -parent $yafAngularCreateButton -label "Create Angular Camera" -width 200`;
				         //-command "columnLayout -edit -enable true $yafAngularDetail";
                separator -parent $yafAngular -height 15 -style "in";
                string $yafAngularDetail = `columnLayout -parent $yafAngular -enable false`;
                  checkBoxGrp -parent $yafAngularDetail -label "" -label1 "Mirrored" -numberOfCheckBoxes 1;
                  checkBoxGrp -parent $yafAngularDetail -label "" -label1 "Circular" -numberOfCheckBoxes 1;
                  floatSliderGrp -parent $yafAngularDetail -label "Angle" -minValue 0.0 -maxValue 180.0 -field true;
                  floatSliderGrp -parent $yafAngularDetail -label "Max Angle" -minValue 0.0 -maxValue 180.0 -field true;
                separator -parent $yafAngular -height 15 -style "in";
                
              string $yafArchitect = `frameLayout -parent $yafCameraScroll -label "Architect Camera" -width 390 -collapsable 1`;
                string $yafArchitectCreateButton = `columnLayout -parent $yafArchitect`;
                  string $yafArchitectButton=`button -parent $yafArchitectCreateButton -label "Create Architect Camera" -width 200`;
				         //-command "columnLayout -edit -enable true $yafArchDetail";
                separator -parent $yafArchitect -height 15 -style "in";
                string $yafArchDetail = `columnLayout -parent $yafArchitect`;
                  attrEnumOptionMenuGrp -parent $yafArchDetail -label "Bokeh Type" //here needed an attribute
                                        -enumeratedItem 0 "disk1"
                                        -enumeratedItem 1 "disk2"
                                        -enumeratedItem 2 "triangle"
                                        -enumeratedItem 3 "square"
                                        -enumeratedItem 4 "pentagon"
                                        -enumeratedItem 5 "hexagon"
                                        -enumeratedItem 6 "ring";
                  floatSliderGrp -parent $yafArchDetail -label "Bokeh Rtation" -minValue 0.0 -maxValue 180.0 -field true;
                  intSliderGrp -parent $yafArchDetail -label "Aperture Size" -minValue 0 -maxValue 20 -field true;
                  attrEnumOptionMenuGrp -parent $yafArchDetail -label "Bokeh Bias"
				                        -enumeratedItem 0 "uniform"
										-enumeratedItem 1 "edge"
										-enumeratedItem 2 "center";
				  floatSliderGrp -parent $yafArchDetail -label "Dof Distance" -minValue 0.0 -maxValue 1000.0 -field true;
				  checkBoxGrp -parent $yafArchDetail -numberOfCheckBoxes 1 -label "" -label1 "Object Focus"
				              -onCommand1 "textFieldGrp -edit -enable true $yafPerspText"
							  -offCommand1 "textFieldGrp -edit -enable false $yafPerspText";
				  string $yafPerspText = `textFieldGrp -parent $yafArchDetail -label "Dof Object" -editable true -enable false`;  

                separator -parent $yafArchitect -height 15 -style "in";
            button -edit
                   -command ("columnLayout -edit -enable true "+$yafPerspDetail)
                   $yafPerspButton;
            checkBoxGrp -edit
                        -onCommand1 ("textFieldGrp -edit -enable true "+$yafPerspText)
						-offCommand1 ("textFieldGrp -edit -enable false "+$yafPerspText)
						$yafFocusCheckbox;
			button -edit
                   -command ("columnLayout -edit -enable true "+$yafOrthoDetail)
                   $yafOrthoButton;
            button -edit
                   -command ("columnLayout -edit -enable true "+$yafAngularDetail)
                   $yafAngularButton;
            button -edit
                   -command ("columnLayout -edit -enable true "+$yafArchDetail)
                   $yafArchitectButton;
}
global proc yWorldSettingUI(string $worldSettingTab)
{
	string $nodeTemp[]=`ls -type "yafWorldSetting"`;
	string $yafWorldSettingNode;
	if(size($nodeTemp)==0)
	      {
				$yafWorldSettingNode = `createNode yafWorldSetting`;
				select -deselect $yafWorldSettingNode;
		  }
	else
	  {$yafWorldSettingNode=$nodeTemp[0];}
		    
	string $yafWorldScroll = `scrollLayout -parent $worldSettingTab -width 420 -height 550`;
	  separator -parent $yafWorldScroll -height 10 -style "none";
            text -parent $yafWorldScroll -label "     this menu is for setting background and volume";
      separator -parent $yafWorldScroll -height 10 -style "none";
      
     string $yafWVolume=`frameLayout -parent $yafWorldScroll -label "Volume Setting" -collapsable 1`;
         separator -parent $yafWVolume -height 15 -style "in";
         string $yafVolumeEnum=`attrEnumOptionMenu -parent $yafWVolume -label "         Volume Type" -attribute ($yafWorldSettingNode+".VolumeTypes")`;
         int $yafVolumeType=`getAttr ($yafWorldSettingNode+".VolumeTypes")`;
		 string $yafVolumeNone;
		 if($yafVolumeType==0)
		 {$yafVolumeNone = `frameLayout -parent $yafWVolume -label "None" -width 390 -collapsable 1 -collapse false -enable true`;}
	     else
	     {$yafVolumeNone = `frameLayout -parent $yafWVolume -label "None" -width 390 -collapsable 1 -collapse true -enable false`;}
	         attrControlGrp -attribute ($yafWorldSettingNode+".StepSize");
         string $yafVolumeScatter;
         if($yafVolumeType==1)
		 {$yafVolumeScatter = `frameLayout -parent $yafWVolume -label "Single Scatter" -width 390 -collapsable 1 -collapse false -enable true`;}
	     else
	     {$yafVolumeScatter = `frameLayout -parent $yafWVolume -label "Single Scatter" -width 390 -collapsable 1 -collapse true -enable false`;}
             attrControlGrp -attribute ($yafWorldSettingNode+".Adaptive");
             string $yafOptimizeControl=`checkBoxGrp -parent $yafVolumeScatter -numberOfCheckBoxes 1 -label "" -label1 "Optimize"`;
             connectControl -index 2 $yafOptimizeControl ($yafWorldSettingNode+".Optimize") ;
             string $yafAttGridColumn;
             if(`getAttr ($yafWorldSettingNode+".Optimize")`==1)
             {$yafAttGridColumn=`columnLayout -parent $yafVolumeScatter -enable true`;}
             else
             {$yafAttGridColumn=`columnLayout -parent $yafVolumeScatter -enable false`;}
                 attrControlGrp -attribute ($yafWorldSettingNode+".AttGridResolution");
		 string $yafVolumeSky;
         if($yafVolumeType==2)
		 {$yafVolumeSky = `frameLayout -parent $yafWVolume -label "Sky" -width 390 -collapsable 1 -collapse false -enable true`;}
	     else
	     {$yafVolumeSky = `frameLayout -parent $yafWVolume -label "Sky" -width 390 -collapsable 1 -collapse true -enable false`;}
	         attrControlGrp -attribute ($yafWorldSettingNode+".Scale");
	         attrControlGrp -attribute ($yafWorldSettingNode+".Alpha");
         separator -parent $yafWVolume -height 15 -style "in";
         //events in this framelayout
         attrEnumOptionMenu -edit
		                     -changeCommand ("yVolumeChange"+" "+$yafWorldSettingNode+" "+$yafVolumeNone+" "+$yafVolumeScatter+" "+$yafVolumeSky)
		                     $yafVolumeEnum;
		 checkBoxGrp -edit
		             -onCommand1 ("columnLayout -edit -enable true "+$yafAttGridColumn)
					 -offCommand1 ("columnLayout -edit -enable false "+$yafAttGridColumn)
					 $yafOptimizeControl;
	 string $yafBG = `frameLayout -parent $yafWorldScroll -label "Background Settings"-collapsable 1`;
      separator -parent $yafBG -height 10 -style "none";
      string $yafBackGType=`attrEnumOptionMenu -parent $yafBG -label "         Background Type" -attribute ($yafWorldSettingNode+".BackgroundTypes")`;
//	                        -changeCommand ("backgroundChange"+" "+$yafWorldSettingNode)`;
	  separator -parent $yafBG -height 15 -style "in";
	  attrControlGrp -attribute ($yafWorldSettingNode+".BackgroundPower");
	  separator -parent $yafBG -height 15 -style "in";
	  //this is for draw the frames first
	  int $yafBGTypeTest=`getAttr ($yafWorldSettingNode+".BackgroundTypes")`;
	  string $yafSingleColor;
	  if($yafBGTypeTest==0)	  
	  {$yafSingleColor = `frameLayout -parent $yafBG -label "Single Color Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
	  else
	    {$yafSingleColor = `frameLayout -parent $yafBG -label "Single Color Setting" -width 390 -collapsable 1 -collapse true -enable false`;}
	      separator -parent $yafSingleColor -height 15 -style "in";
	      attrControlGrp -attribute ($yafWorldSettingNode+".BackgroundColor");
//	      string $yafIBLControl=`checkBoxGrp -parent $yafSingleColor -numberOfCheckBoxes 1 -label "" -label1 "Use IBL" 
//		                                     -value1 ($yafWorldSettingNode+".UseIBL")`;
//	      string $yafIBLControl=`attrControlGrp -attribute ($yafWorldSettingNode+".UseIBL") -label "Use IBL"`;
          string $yafSingleIBLControl=`checkBoxGrp -parent $yafSingleColor -numberOfCheckBoxes 1 -label "" -label1 "Use IBL"`;
          connectControl -index 2 $yafSingleIBLControl ($yafWorldSettingNode+".UseIBL") ;
//	      string $yafIBLSamples=`attrControlGrp -attribute ($yafWorldSettingNode+".IBLSamples") -label "IBL Samples" -enable false`;
//	      if(`getAttr ($yafWorldSettingNode+".UseIBL")`==1 )
//	         {attrControlGrp -edit -enable true
//                                               $yafIBLSamples;}
//	        else 
//	          {attrControlGrp -edit -enable false
//                                               $yafIBLSamples;}
          string $yafSingleIBLSamples;
	      if(`getAttr ($yafWorldSettingNode+".UseIBL")`==1 )
	         {$yafSingleIBLSamples=`attrControlGrp -attribute ($yafWorldSettingNode+".IBLSamples") -label "IBL Samples" -enable true`;}
	        else 
	          {$yafSingleIBLSamples=`attrControlGrp -attribute ($yafWorldSettingNode+".IBLSamples") -label "IBL Samples" -enable false`;}
	      separator -parent $yafSingleColor -height 15 -style "in";
	      checkBoxGrp -edit
                        -onCommand1 ("attrControlGrp -edit -enable true "+$yafSingleIBLSamples)
						-offCommand1 ("attrControlGrp -edit -enable false "+$yafSingleIBLSamples)
						$yafSingleIBLControl;

//	      int $yafIBLBool=(!($yafWorldSettingNode+".UseIBL"));
//		  attrControlGrp -edit 
//					     -changeCommand ("changeIBL"+$yafWorldSettingNode+" "+$yafIBLSamples)
//						 $yafIBLControl; 
//           checkBoxGrp -edit
//                        -onCommand1 ("attrControlGrp -edit -enable true "+$yafIBLSamples)
//						-offCommand1 ("attrControlGrp -edit -enable false "+$yafIBLSamples)
//						$yafIBLControl;
          //setParent ..;

      string $yafGradient;
      if($yafBGTypeTest==1)	  
	  {$yafGradient = `frameLayout -parent $yafBG -label "Gradient Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
	  else
	    {$yafGradient = `frameLayout -parent $yafBG -label "Gradient Setting" -width 390 -collapsable 1 -collapse true -enable false`;}
//	  string $yafGradient = `frameLayout -parent $yafBG -label "Gradient Setting" -width 390 -collapsable 1 -enable false -collapse true`;
          separator -parent $yafGradient -height 15 -style "in";
          attrControlGrp -attribute ($yafWorldSettingNode+".HorizonColor");
          attrControlGrp -attribute ($yafWorldSettingNode+".HorGroundColor");
          attrControlGrp -attribute ($yafWorldSettingNode+".ZenithColor");
          attrControlGrp -attribute ($yafWorldSettingNode+".ZenGroundColor");
          string $yafGradientIBLControl=`checkBoxGrp -parent $yafGradient -numberOfCheckBoxes 1 -label "" -label1 "Use IBL"`;
          connectControl -index 2 $yafGradientIBLControl ($yafWorldSettingNode+".UseIBL") ;
          string $yafGradientIBLSamples;
	      if(`getAttr ($yafWorldSettingNode+".UseIBL")`==1 )
	         {$yafGradientIBLSamples=`attrControlGrp -attribute ($yafWorldSettingNode+".IBLSamples") -label "IBL Samples" -enable true`;}
	        else 
	          {$yafGradientIBLSamples=`attrControlGrp -attribute ($yafWorldSettingNode+".IBLSamples") -label "IBL Samples" -enable false`;}
	      separator -parent $yafSingleColor -height 15 -style "in";
	      checkBoxGrp -edit
                        -onCommand1 ("attrControlGrp -edit -enable true "+$yafGradientIBLSamples)
						-offCommand1 ("attrControlGrp -edit -enable false "+$yafGradientIBLSamples)
						$yafGradientIBLControl;
          separator -parent $yafGradient -height 15 -style "in";
          
          //here IBL again
          //setParent ..;
      string $yafTexture;
      if($yafBGTypeTest==2)	  
	  {$yafTexture = `frameLayout -parent $yafBG -label "Texture Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
	  else
	    {$yafTexture = `frameLayout -parent $yafBG -label "Texture Setting" -width 390 -collapsable 1 -collapse true -enable false`;}          
//      string $yafTexture = `frameLayout -parent $yafBG -label "Texture Setting" -width 390 -collapsable 1 -enable false -collapse true`;
          separator -parent $yafTexture -height 15 -style "in";
          attrControlGrp -attribute ($yafWorldSettingNode+".TextureRotation");
          string $yafTextureIBLControl=`checkBoxGrp -parent $yafTexture -numberOfCheckBoxes 1 -label "" -label1 "Use IBL"`;
          connectControl -index 2 $yafTextureIBLControl ($yafWorldSettingNode+".UseIBL") ;
          string $yafTextureIBLSetting;
          if(`getAttr ($yafWorldSettingNode+".UseIBL")`==1 )
	         {$yafTextureIBLSetting=`columnLayout -parent $yafTexture -enable true`;}
	        else 
	          {$yafTextureIBLSetting=`columnLayout -parent $yafTexture -enable false`;}
	          attrControlGrp -attribute ($yafWorldSettingNode+".IBLSamples") -label "IBL Samples";
	          attrControlGrp -attribute ($yafWorldSettingNode+".DiffusePhotons");
	          attrControlGrp -attribute ($yafWorldSettingNode+".CausticPhotons");
	      checkBoxGrp -edit
                        -onCommand1 ("columnLayout -edit -enable true "+$yafTextureIBLSetting)
						-offCommand1 ("columnLayout -edit -enable false "+$yafTextureIBLSetting)
						$yafTextureIBLControl;
          
          
          separator -parent $yafTexture -height 15 -style "in";
          
          //here IBL again
          //setParent ..;
      string $yafSunsky;
      if($yafBGTypeTest==3)	  
	  {$yafSunsky = `frameLayout -parent $yafBG -label "Sunsky Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
	  else
	    {$yafSunsky = `frameLayout -parent $yafBG -label "Sunsky Setting" -width 390 -collapsable 1 -collapse true -enable false`;}          
//      string $yafSunsky = `frameLayout -parent $yafBG -label "Sunsky Setting" -width 390 -collapsable 1 -enable false -collapse true`;
          separator -parent $yafSunsky -height 15 -style "in";
		  attrControlGrp -attribute ($yafWorldSettingNode+".Turbidity");
          attrControlGrp -attribute ($yafWorldSettingNode+".AHorBrght") -label "A(HorBrght)";
          attrControlGrp -attribute ($yafWorldSettingNode+".BHorSprd") -label "B(HorSprd)";
          attrControlGrp -attribute ($yafWorldSettingNode+".CSunBrght") -label "C(SunBrght)";
          attrControlGrp -attribute ($yafWorldSettingNode+".DSunsize") -label "D(Sunsize)";
          attrControlGrp -attribute ($yafWorldSettingNode+".EBacklight") -label "E(Backlight)";
          attrControlGrp -attribute ($yafWorldSettingNode+".xDirection");
          attrControlGrp -attribute ($yafWorldSettingNode+".yDirection");
          attrControlGrp -attribute ($yafWorldSettingNode+".zDirection");
          button -label "Get angle from selected sun lamp";
          button -label "Get position from selected sun lamp";
          button -label "Update angle and position of selected sun lamp";
          attrControlGrp -attribute ($yafWorldSettingNode+".AddRealSun");
          attrControlGrp -attribute ($yafWorldSettingNode+".SunPower");
          attrControlGrp -attribute ($yafWorldSettingNode+".Skylight");
          attrControlGrp -attribute ($yafWorldSettingNode+".SkySamples");
          separator -parent $yafSunsky -height 15 -style "in";
          //setParent ..;
      string $yafDSSunsky;
      if($yafBGTypeTest==4)	  
	  {$yafDSSunsky= `frameLayout -parent $yafBG -label "DarkTide's Sunsky Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
	  else
	    {$yafDSSunsky = `frameLayout -parent $yafBG -label "DarkTide's Sunsky Setting" -width 390 -collapsable 1 -collapse true -enable false`;}           
//      string $yafDSSunsky = `frameLayout -parent $yafBG -label "DarkTide's Sunsky Setting" -width 390 -collapsable 1 -enable false -collapse true`;
          separator -parent $yafDSSunsky -height 15 -style "in";
		  attrControlGrp -attribute ($yafWorldSettingNode+".DarkTideTurbidity") -label "DarkTide's Turbidity";
          attrControlGrp -attribute ($yafWorldSettingNode+".BrightnessOfHorizonGradient") -label "Brightness Of Hor. Gradient";
          attrControlGrp -attribute ($yafWorldSettingNode+".LuminanceOfHorizon");
          attrControlGrp -attribute ($yafWorldSettingNode+".SolarRegionIntensity");
          attrControlGrp -attribute ($yafWorldSettingNode+".WidthOfCircumsolarRegion");
          attrControlGrp -attribute ($yafWorldSettingNode+".BackgroundLight");
          attrControlGrp -attribute ($yafWorldSettingNode+".ClampRGB");
          attrControlGrp -attribute ($yafWorldSettingNode+".xDirection");
          attrControlGrp -attribute ($yafWorldSettingNode+".yDirection");
          attrControlGrp -attribute ($yafWorldSettingNode+".zDirection");
          button -label "Get angle from selected sun lamp";
          button -label "Get position from selected sun lamp";
          button -label "Update angle and position of selected sun lamp";
          attrControlGrp -attribute ($yafWorldSettingNode+".Altitude");
          attrControlGrp -attribute ($yafWorldSettingNode+".Night");         
          separator -parent $yafDSSunsky -height 15 -style "in";
          string $yafDSRealSunControl=`checkBoxGrp -parent $yafDSSunsky -numberOfCheckBoxes 1 -label "" -label1 "Add DS Real Sun"`;
          connectControl -index 2 $yafDSRealSunControl ($yafWorldSettingNode+".AddDSRealSun") ;
          string $yafDSSunPower;
	      if(`getAttr ($yafWorldSettingNode+".AddDSRealSun")`==1 )
	         {$yafDSSunPower=`attrControlGrp -attribute ($yafWorldSettingNode+".DSSunPower") -label "DS Sun Power" -enable true`;}
	        else 
	          {$yafDSSunPower=`attrControlGrp -attribute ($yafWorldSettingNode+".DSSunPower") -label "DS Sun Power" -enable false`;}
//          attrControlGrp -attribute ($yafWorldSettingNode+".AddDSRealSun");
//          attrControlGrp -attribute ($yafWorldSettingNode+".DSSunPower");
          separator -parent $yafDSSunsky -height 15 -style "in";
          string $yafDSSkyLightControl=`checkBoxGrp -parent $yafDSSunsky -numberOfCheckBoxes 1 -label "" -label1 "Add DS Sun Light"`;
          connectControl -index 2 $yafDSSkyLightControl ($yafWorldSettingNode+".AddDSSkyLight");
          string $yafDSSkySetting;
          if(`getAttr ($yafWorldSettingNode+".AddDSSkyLight")`==1 )
	         {$yafDSSkySetting=`columnLayout -parent $yafDSSunsky -enable true`;}
	        else 
	          {$yafDSSkySetting=`columnLayout -parent $yafDSSunsky -enable false`;}     
//          attrControlGrp -attribute ($yafWorldSettingNode+".AddDSSkyLight");
          attrControlGrp -attribute ($yafWorldSettingNode+".DSSkyPower") -label "DS Sky Power";
          attrControlGrp -attribute ($yafWorldSettingNode+".DiffusePhotons");
	      attrControlGrp -attribute ($yafWorldSettingNode+".CausticPhotons");
	      setParent ..; 
		  separator -parent $yafDSSunsky -height 15 -style "in";
	      string $yafDSSkySamples;
		  if((`getAttr ($yafWorldSettingNode+".AddDSRealSun")`==0)&&(`getAttr ($yafWorldSettingNode+".AddDSSkyLight")`==0))   
	         {$yafDSSkySamples=`attrControlGrp -attribute ($yafWorldSettingNode+".DSSkySamples") -label "DS Sun Samples" -enable false`;}
	        else 
	          {$yafDSSkySamples=`attrControlGrp -attribute ($yafWorldSettingNode+".DSSkySamples") -label "DS Sun Samples" -enable true`;}		       
//          attrControlGrp -attribute ($yafWorldSettingNode+".DSSkySamples");
          separator -parent $yafDSSunsky -height 15 -style "in";          
          attrControlGrp -attribute ($yafWorldSettingNode+".Brightness");
          attrControlGrp -attribute ($yafWorldSettingNode+".Exposure");
          attrControlGrp -attribute ($yafWorldSettingNode+".GammaEncoding");
          attrEnumOptionMenuGrp -label "DSSky Color Spaces" 
		                        -attribute ($yafWorldSettingNode+".DarkTideSunskyColorSpaces") 
								-enumeratedItem 0 "CIE (E)" 
								-enumeratedItem 1 "CIE (D50)" 
								-enumeratedItem 2 "sRBG (D65)" 
								-enumeratedItem 3 "sRGB (D50)";        
          separator -parent $yafDSSunsky -height 15 -style "in";
          
          checkBoxGrp -edit
                        -onCommand1 ("DSSunControlOn"+" "+$yafDSSunPower+" "+$yafDSSkySamples+" "+$yafWorldSettingNode)
						-offCommand1 ("DSSunControlOff"+" "+$yafDSSunPower+" "+$yafDSSkySamples+" "+$yafWorldSettingNode)
						$yafDSRealSunControl;
          checkBoxGrp -edit
                       -onCommand1 ("DSSkyControlOn"+" "+$yafDSSkySetting+" "+$yafDSSkySamples+" "+$yafWorldSettingNode)
						-offCommand1 ("DSSkyControlOff"+" "+$yafDSSkySetting+" "+$yafDSSkySamples+" "+$yafWorldSettingNode)
						$yafDSSkyLightControl;
						
		  attrEnumOptionMenu -edit
		                     -changeCommand ("backgroundChange"+" "+$yafWorldSettingNode+" "+$yafSingleColor+" "+$yafGradient+" "+$yafTexture+" "+$yafSunsky+" "+$yafDSSunsky)
		                     $yafBackGType;
          //setParent ..;

}
global proc DSSunControlOn(string $Sunpower,string $Sunsamples,string $nodeName)
{
	attrControlGrp -edit -enable true $Sunpower;
//	if(`attrControlGrp -q -enable $Sunsamples`==false)
    if(`getAttr ($nodeName+".AddDSSkyLight")`==0)
	   attrControlGrp -edit -enable true $Sunsamples;
}
global proc DSSunControlOff(string $Sunpower,string $Sunsamples,string $nodeName)
{
	attrControlGrp -edit -enable false $Sunpower;
	if(`getAttr ($nodeName+".AddDSSkyLight")`==0)
	   attrControlGrp -edit -enable false $Sunsamples;
}
global proc DSSkyControlOn(string $Skysetting,string $Sunsamples,string $nodeName)
{
	columnLayout -edit -enable true $Skysetting;
	if(`getAttr ($nodeName+".AddDSRealSun")`==0)
	   attrControlGrp -edit -enable true $Sunsamples;
}
global proc DSSkyControlOff(string $Skysetting,string $Sunsamples,string $nodeName)
{
	columnLayout -edit -enable false $Skysetting;
	if(`getAttr ($nodeName+".AddDSRealSun")`==0)
	   attrControlGrp -edit -enable false $Sunsamples;
}
global proc backgroundChange(string $nodeName,string $frameSingle,string $frameGra,string $frameTex,string $frameSun,string $frameDT)
{
	int $type;
	$type=`getAttr ($nodeName+".BackgroundTypes")`;
	switch($type)
	{
		case 0:
			frameLayout -edit -enable true -collapse false $frameSingle;
			frameLayout -edit -enable false -collapse true $frameGra;
			frameLayout -edit -enable false -collapse true $frameTex;
			frameLayout -edit -enable false -collapse true $frameSun;
			frameLayout -edit -enable false -collapse true $frameDT;
			break;
		case 1:
			frameLayout -edit -enable false -collapse true $frameSingle;
			frameLayout -edit -enable true -collapse false $frameGra;
			frameLayout -edit -enable false -collapse true $frameTex;
			frameLayout -edit -enable false -collapse true $frameSun;
			frameLayout -edit -enable false -collapse true $frameDT;
			break;
		case 2:
			frameLayout -edit -enable false -collapse true $frameSingle;
			frameLayout -edit -enable false -collapse true $frameGra;
			frameLayout -edit -enable true -collapse false $frameTex;
			frameLayout -edit -enable false -collapse true $frameSun;
			frameLayout -edit -enable false -collapse true $frameDT;
			break;
		case 3:
			frameLayout -edit -enable false -collapse true $frameSingle;
			frameLayout -edit -enable false -collapse true $frameGra;
			frameLayout -edit -enable false -collapse true $frameTex;
			frameLayout -edit -enable true -collapse false $frameSun;
			frameLayout -edit -enable false -collapse true $frameDT;
			break;
		case 4:
			frameLayout -edit -enable false -collapse true $frameSingle;
			frameLayout -edit -enable false -collapse true $frameGra;
			frameLayout -edit -enable false -collapse true $frameTex;
			frameLayout -edit -enable false -collapse true $frameSun;
			frameLayout -edit -enable true -collapse false $frameDT;
			break;
		default:
			print "nothing";
			break;
	}
}
global proc yVolumeChange(string $nodeName,string $none,string $scatter,string $sky)
{
	int $volumeType=`getAttr ($nodeName+".VolumeTypes")`;
	switch($volumeType)
	{
	    case 0:
			frameLayout -edit -enable true -collapse false $none;
			frameLayout -edit -enable false -collapse true $scatter;
			frameLayout -edit -enable false -collapse true $sky;
			break;
		case 1:
			frameLayout -edit -enable false -collapse true $none;
			frameLayout -edit -enable true -collapse false $scatter;
			frameLayout -edit -enable false -collapse true $sky;
			break;
		case 2:
			frameLayout -edit -enable false -collapse true $none;
			frameLayout -edit -enable false -collapse true $scatter;
			frameLayout -edit -enable true -collapse false $sky;
			break;
		case 3:
			break;	
	}
}

//global proc int testUseIBL(string $yafWorldSettingNode)
//{

//    int $temp;
//    $temp=(`getAttr ($yafWorldSettingNode+".UseIBL")`)?0:1;
//    return $temp;
	
//}
//global proc changeIBL(string $nodeName,string $attrName)
//{
//		$temp=`attrControlGrp -q -enable $attrName`;
//		if($temp==1 )
//	         {
//					attrControlGrp -edit -enable false $attrName;
//					print "command excuted";
//			 }
//	        else 
//	          {  
//					attrControlGrp -edit -enable true $attrName; 
//					print "command excuted too";
//			  }
//}
global proc yIntegratorSettingUI(string $yafIntegratorSetting, string $yafRenderSettingNode)
{	
	$yafRenderScroll=`scrollLayout -parent $yafIntegratorSetting -width 420 -height 550`;
	    separator -parent $yafRenderScroll -height 10 -style "none";
		text -parent $yafRenderScroll -label "     this menu is for integrator settings";
        separator -parent $yafRenderScroll -height 10 -style "none";
	        
	    string $yafLightingMethodSetting=`frameLayout -parent $yafRenderScroll -label "Lighting Method Setting" -width 390 -collapsable 1`;
	        separator -parent $yafLightingMethodSetting -height 15 -style "in";
	        string $yafLightingTypeEnum=`attrEnumOptionMenu -parent $yafLightingMethodSetting -label "           Lighting Type" -attribute ($yafRenderSettingNode+".LightingType")`;
	        int $yafLightingType=`getAttr ($yafRenderSettingNode+".LightingType")`;
	        string $yafDirLight;
	        if($yafLightingType==0)
	        {$yafDirLight=`frameLayout -parent $yafLightingMethodSetting -label "Direct Light Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
			else
			{$yafDirLight=`frameLayout -parent $yafLightingMethodSetting -label "Direct Light Setting" -width 390 -collapsable 1 -collapse true -enable false`;}
			    string $yafUseCausticControl=`checkBoxGrp -parent $yafDirLight -numberOfCheckBoxes 1 -label "" -label1 "Use Caustic"`;
			    connectControl -index 2 $yafUseCausticControl ($yafRenderSettingNode+".UseCaustic");
				string $yafUseCaustic;
				if(`getAttr ($yafRenderSettingNode+".UseCaustic")`==1)
				{$yafUseCaustic=`columnLayout -parent $yafDirLight -enable true`;}
				else
				{$yafUseCaustic=`columnLayout -parent $yafDirLight -enable false`;}
				    attrControlGrp -attribute ($yafRenderSettingNode+".Photons");
				    attrControlGrp -attribute ($yafRenderSettingNode+".CausticMix");
				    attrControlGrp -attribute ($yafRenderSettingNode+".CausticDepth");
				    attrControlGrp -attribute ($yafRenderSettingNode+".DirCausticRadius");				    
				string $yafUseAOControl=`checkBoxGrp -parent $yafDirLight -numberOfCheckBoxes 1 -label "" -label1 "Use AO"`;
				connectControl -index 2 $yafUseAOControl ($yafRenderSettingNode+".UseAO");
				string $yafUseAO;
				if(`getAttr ($yafRenderSettingNode+".UseAO")`==1)
				{$yafUseAO=`columnLayout -parent $yafDirLight -enable true`;}
				else
				{$yafUseAO=`columnLayout -parent $yafDirLight -enable false`;}
				    attrControlGrp -attribute ($yafRenderSettingNode+".AOSamples");
				    attrControlGrp -attribute ($yafRenderSettingNode+".AODistance");
				    attrControlGrp -attribute ($yafRenderSettingNode+".AOColor");
	        string $yafPathTracing;
	        if($yafLightingType==1)
	        {$yafPathTracing=`frameLayout -parent $yafLightingMethodSetting -label "Path Tracing Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
			else
			{$yafPathTracing=`frameLayout -parent $yafLightingMethodSetting -label "Path Tracing Setting" -width 390 -collapsable 1 -collapse true -enable false`;}
			    string $yafCausticEnum=`attrEnumOptionMenu -parent $yafPathTracing -label "           Caustic Method" -attribute ($yafRenderSettingNode+".CausticTypes")`;
			    int $yafCausticType=`getAttr ($yafRenderSettingNode+".CausticTypes")`;
			    string $yafPathPhoton;
			    if(($yafCausticType==2)||($yafCausticType==3))
				{$yafPathPhoton=`columnLayout -parent $yafPathTracing -enable true`;}
				else
				{$yafPathPhoton=`columnLayout -parent $yafPathTracing -enable false`;}
				    attrControlGrp -attribute ($yafRenderSettingNode+".Photons");
				    attrControlGrp -attribute ($yafRenderSettingNode+".CausticMix");
				    attrControlGrp -attribute ($yafRenderSettingNode+".CausticDepth");
				    attrControlGrp -attribute ($yafRenderSettingNode+".DirCausticRadius");
					setParent ..;
				attrControlGrp -attribute ($yafRenderSettingNode+".GIDepth");
				attrControlGrp -attribute ($yafRenderSettingNode+".PathSamples");	
				attrControlGrp -attribute ($yafRenderSettingNode+".NoRecursive");					    
	        string $yafPhotonMapping;
	        if($yafLightingType==2)
	        {$yafPhotonMapping=`frameLayout -parent $yafLightingMethodSetting -label "Photon Mapping Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
			else
			{$yafPhotonMapping=`frameLayout -parent $yafLightingMethodSetting -label "Photon Mapping Setting" -width 390 -collapsable 1 -collapse true -enable false`;}
			    attrControlGrp -attribute ($yafRenderSettingNode+".GIDepth")-label "GI Depth";
			    attrControlGrp -attribute ($yafRenderSettingNode+".Photons") -label "Diffuse Photons";
			    attrControlGrp -attribute ($yafRenderSettingNode+".DiffuseRadius");	
				attrControlGrp -attribute ($yafRenderSettingNode+".CausticPhotons");	
			    attrControlGrp -attribute ($yafRenderSettingNode+".PhotonCausticRadius") -label "Caustic Radius";	
			    attrControlGrp -attribute ($yafRenderSettingNode+".CausticMix");
			    attrControlGrp -attribute ($yafRenderSettingNode+".Search");	
			    attrControlGrp -attribute ($yafRenderSettingNode+".FinalGather");	
			    attrControlGrp -attribute ($yafRenderSettingNode+".FGBounces") -label "FG Bounces";	
			    attrControlGrp -attribute ($yafRenderSettingNode+".FGSamples") -label "FG Samples";	
			    attrControlGrp -attribute ($yafRenderSettingNode+".ShowMap");	
	        string $yafDebugRender;
	        if($yafLightingType==3)
	        {$yafDebugRender=`frameLayout -parent $yafLightingMethodSetting -label "Debug Setting" -width 390 -collapsable 1 -collapse false -enable true`;}
			else
			{$yafDebugRender=`frameLayout -parent $yafLightingMethodSetting -label "Debug Setting" -width 390 -collapsable 1 -collapse true -enable false`;}
			    attrControlGrp -attribute ($yafRenderSettingNode+".DebugType");
			    attrControlGrp -attribute ($yafRenderSettingNode+".PerturbedNormals");
	        separator -parent $yafLightingMethodSetting -height 15 -style "in";
	        //GUI events for lighting type framelayout
	        attrEnumOptionMenu -edit
	                           -changeCommand ("yLightingTypeChange"+" "+$yafRenderSettingNode+" "+$yafDirLight+" "+$yafPathTracing+" "+$yafPhotonMapping+" "+$yafDebugRender)
	                           $yafLightingTypeEnum;
	        checkBoxGrp -edit
			            -onCommand1 ("columnLayout -edit -enable true "+$yafUseCaustic)
						-offCommand1 ("columnLayout -edit -enable false "+$yafUseCaustic)    
						$yafUseCausticControl;
	        checkBoxGrp -edit
			            -onCommand1 ("columnLayout -edit -enable true "+$yafUseAO)
						-offCommand1 ("columnLayout -edit -enable false "+$yafUseAO)    
						$yafUseAOControl;
			attrEnumOptionMenu -edit
	                           -changeCommand ("yCausticEnumChange"+" "+$yafRenderSettingNode+" "+$yafPathPhoton)
	                           $yafCausticEnum;

}
global proc yOutputMethodChange(string $nodeName,string $gui,string $image,string $xml)
{
	int $type;
	$type=`getAttr ($nodeName+".OutputMethod")`;
	switch($type)
	{
		case 0:
			frameLayout -edit -enable true -collapse false $gui;
			frameLayout -edit -enable false -collapse true $image;
			frameLayout -edit -enable false -collapse true $xml;
			break;
		case 1:
			frameLayout -edit -enable false -collapse true $gui;
			frameLayout -edit -enable true -collapse false $image;
			frameLayout -edit -enable false -collapse true $xml;
			break;
		case 2:
			frameLayout -edit -enable false -collapse true $gui;
			frameLayout -edit -enable false -collapse true $image;
			frameLayout -edit -enable true -collapse false $xml;
			break;
		default:
			break;			
	}
	
}
global proc yAAPassChange(string $nodeName,string $columnName)
{
	int $AAPass;
	$AAPass=`getAttr ($nodeName+".AAPasses")`;
	if($AAPass>1)
	columnLayout -edit -enable true $columnName;
	else
	columnLayout -edit -enable false $columnName;	
}
global proc yLightingTypeChange(string $nodeName,string $direct,string $pathtracing,string $photon,string $debug)
{
	int $lightType;
	$lightType=`getAttr ($nodeName+".LightingType")`;
	switch($lightType)
	{
		case 0:
			frameLayout -edit -enable true -collapse false $direct;
			frameLayout -edit -enable false -collapse true $pathtracing;
			frameLayout -edit -enable false -collapse true $photon;
			frameLayout -edit -enable false -collapse true $debug;
			break;
		case 1:
			frameLayout -edit -enable false -collapse true $direct;
			frameLayout -edit -enable true -collapse false $pathtracing;
			frameLayout -edit -enable false -collapse true $photon;
			frameLayout -edit -enable false -collapse true $debug;
			break;
		case 2:
			frameLayout -edit -enable false -collapse true $direct;
			frameLayout -edit -enable false -collapse true $pathtracing;
			frameLayout -edit -enable true -collapse false $photon;
			frameLayout -edit -enable false -collapse true $debug;
			break;
		case 3:
			frameLayout -edit -enable false -collapse true $direct;
			frameLayout -edit -enable false -collapse true $pathtracing;
			frameLayout -edit -enable false -collapse true $photon;
			frameLayout -edit -enable true -collapse false $debug;
			break;
		default:
			break;
	}
}

global proc yCausticEnumChange(string $nodeName,string $columnName)
{
	int $causticType;
	$causticType=`getAttr ($nodeName+".CausticTypes")`;
	switch($causticType)
	{
		case 0:
			columnLayout -edit -enable false $columnName;
			break;
		case 1:
			columnLayout -edit -enable false $columnName;
			break;
		case 2:
			columnLayout -edit -enable true $columnName;
			break;
		case 3:
			columnLayout -edit -enable true $columnName;
			break;
		default:
			break;
	}

}

global proc yRenderSettingUI(string $renderSettingTab, string $yafRenderSettingNode)
{
	$yafRenderScroll=`scrollLayout -parent $renderSettingTab -width 420 -height 550`;
	    separator -parent $yafRenderScroll -height 10 -style "none";
		text -parent $yafRenderScroll -label "     this menu is for general render settings";
        separator -parent $yafRenderScroll -height 10 -style "none";
        
        string $yafGeneralSetting=`frameLayout -parent $yafRenderScroll -label "General Render Setting" -width 390 -collapsable 1`;
	        separator -parent $yafGeneralSetting -height 15 -style "in";
	        attrControlGrp -attribute ($yafRenderSettingNode+".Gamma");
	        attrControlGrp -attribute ($yafRenderSettingNode+".GammaInput");
	        separator -parent $yafGeneralSetting -height 15 -style "in";
			attrControlGrp -attribute ($yafRenderSettingNode+".RayDepth");
	        attrControlGrp -attribute ($yafRenderSettingNode+".ClayRender");
	        separator -parent $yafGeneralSetting -height 8 -style "in";
	        string $yafTransShadowControl=`checkBoxGrp -parent $yafGeneralSetting -numberOfCheckBoxes 1 -label "" -label1 "Transparent Shadows"`;
            connectControl -index 2 $yafTransShadowControl ($yafRenderSettingNode+".TransparentShadows");	
			
			string $yafShadowDepth;
			if(`getAttr ($yafRenderSettingNode+".TransparentShadows")`==1 )
	         {$yafShadowDepth=`attrControlGrp -attribute ($yafRenderSettingNode+".ShadowDepth") -label "Shadow Depth" -enable true`;}
	        else 
	          {$yafShadowDepth=`attrControlGrp -attribute ($yafRenderSettingNode+".ShadowDepth") -label "Shadow Depth" -enable false`;}
	        separator -parent $yafGeneralSetting -height 8 -style "in";  
			checkBoxGrp -edit
			            -onCommand1 ("attrControlGrp -edit -enable true "+$yafShadowDepth)
						-offCommand1 ("attrControlGrp -edit -enable false "+$yafShadowDepth)    
						$yafTransShadowControl;
			      
	        string $yafAutoThreadsControl=`checkBoxGrp -parent $yafGeneralSetting -numberOfCheckBoxes 1 -label "" -label1 "Auto Threads"`;
	        connectControl -index 2 $yafAutoThreadsControl ($yafRenderSettingNode+".AutoThreads");
			string $yafThreads;
			if(`getAttr ($yafRenderSettingNode+".AutoThreads")`==1 )
	         {$yafThreads=`attrControlGrp -attribute ($yafRenderSettingNode+".Threads") -label "Threads" -enable false`;}
	        else 
	          {$yafThreads=`attrControlGrp -attribute ($yafRenderSettingNode+".Threads") -label "Threads" -enable true`;}	        
	        separator -parent $yafGeneralSetting -height 8 -style "in";
	        checkBoxGrp -edit
			            -onCommand1 ("attrControlGrp -edit -enable false "+$yafThreads)
						-offCommand1 ("attrControlGrp -edit -enable true "+$yafThreads)    
						$yafAutoThreadsControl;
			attrControlGrp -attribute ($yafRenderSettingNode+".RenderZBuffer");
	        separator -parent $yafGeneralSetting -height 15 -style "in";
	     string $yafAASetting=`frameLayout -parent $yafRenderScroll -label "AA setting" -width 390 -collapsable 1`;
	        separator -parent $yafAASetting -height 15 -style "in";
	        string $yafAAPassControl=`attrFieldSliderGrp -parent $yafAASetting -label "AA Passes" -attribute ($yafRenderSettingNode+".AAPasses")`;
	        attrControlGrp -attribute ($yafRenderSettingNode+".AASamples")-label "AA Samples";
	        string $yafAAThrSam;
			if(`getAttr ($yafRenderSettingNode+".AAPasses")`==1 )
			{$yafAAThrSam=`columnLayout -parent $yafAASetting -enable false`;}
			else
			{$yafAAThrSam=`columnLayout -parent $yafAASetting -enable true`;}
	            attrControlGrp -attribute ($yafRenderSettingNode+".AAThreshold") -label "AA Threshhold";
	            attrControlGrp -attribute ($yafRenderSettingNode+".AAIncSamples") -label "AA Inc Samples";
	            setParent ..;
	        attrFieldSliderGrp -edit
	                           -changeCommand ("yAAPassChange"+" "+$yafRenderSettingNode+" "+$yafAAThrSam)
	                           $yafAAPassControl;
	        attrControlGrp -attribute ($yafRenderSettingNode+".FilterType");
	        attrControlGrp -attribute ($yafRenderSettingNode+".AAPixelWidth") -label "AA Pixel Width";
	        separator -parent $yafAASetting -height 15 -style "in";
}

global proc yOutputSettingUI(string $outputSettingTab, string $yafRenderSettingNode)
{
	$yafOutputScroll=`scrollLayout -parent $outputSettingTab -width 420 -height 550`;
	    separator -parent $yafOutputScroll -height 10 -style "none";
		text -parent $yafOutputScroll -label "     this menu is for messy settings...only use pic size now";
        separator -parent $yafOutputScroll -height 10 -style "none";
        
        string $yafSizeSetting=`frameLayout -parent $yafOutputScroll -label "Image Size" -width 390 -collapsable 1`;    
            separator -parent $yafSizeSetting -height 15 -style "in";
	        attrControlGrp -attribute ($yafRenderSettingNode+".width");
	        attrControlGrp -attribute ($yafRenderSettingNode+".height");
	        separator -parent $yafSizeSetting -height 15 -style "in";
        
        string $yafOutputSetting=`frameLayout -parent $yafOutputScroll -label "Output Setting" -width 390 -collapsable 1 -enable false`;        
	        separator -parent $yafOutputSetting -height 15 -style "in";
	        attrControlGrp -attribute ($yafRenderSettingNode+".ClampRGB");
	        attrControlGrp -attribute ($yafRenderSettingNode+".PremultAlpha");
	        string $yafOutMethod=`frameLayout -parent $yafOutputSetting -label "Output Method Setting" -width 390 -collapsable 0`;	            
	            separator -parent $yafOutMethod -height 15 -style "in";
	            button -parent $yafOutMethod -label "Result to Maya";
				string $yafDrawRenderParamControl=`checkBoxGrp -parent $yafOutMethod -numberOfCheckBoxes 1 -label "" -label1 "Draw Render Params"`;
	            connectControl -index 2 $yafDrawRenderParamControl ($yafRenderSettingNode+".DrawRenderParams");
	            string $yafOutCustomString;
	            if(`getAttr ($yafRenderSettingNode+".DrawRenderParams")`==1 )
	            {$yafOutCustomString=`textFieldGrp -parent $yafOutMethod -label "Custom String" -enable true`;}
	            else 
	            {$yafOutCustomString=`textFieldGrp -parent $yafOutMethod -label "Custom String" -enable false`;}
	            checkBoxGrp -edit
			            -onCommand1 ("textFieldGrp -edit -enable true "+$yafOutCustomString)
						-offCommand1 ("textFieldGrp -edit -enable false "+$yafOutCustomString)    
						$yafDrawRenderParamControl;
	            separator -parent $yafOutMethod -height 15 -style "in";
	            string $yafOutMethodEnum=`attrEnumOptionMenu -parent $yafOutMethod -label "                Output Method" -attribute ($yafRenderSettingNode+".OutputMethod")`;
	            int $yafOutMethodTest=`getAttr ($yafRenderSettingNode+".OutputMethod")`;
	            string $yafGUI;
	            if($yafOutMethodTest==0)
	            {$yafGUI=`frameLayout -parent $yafOutMethod -label "GUI Setting" -width 390 -collapsable 0 -collapse false -enable true`;}
	            else
	            {$yafGUI=`frameLayout -parent $yafOutMethod -label "GUI Setting" -width 390 -collapsable 0 -collapse true -enable false`;}
	                attrControlGrp -attribute ($yafRenderSettingNode+".TileOrder");
	                attrControlGrp -attribute ($yafRenderSettingNode+".TileSize");
	                attrControlGrp -attribute ($yafRenderSettingNode+".AutoSave");
	                attrControlGrp -attribute ($yafRenderSettingNode+".AlphaOnAutosave");
	                attrControlGrp -attribute ($yafRenderSettingNode+".ShowResampleMask");
	            string $yafImage;
	            if($yafOutMethodTest==1)
				{$yafImage=`frameLayout -parent $yafOutMethod -label "Image Setting" -width 390 -collapsable 0 -collapse false -enable true`;}
				else
				{$yafImage=`frameLayout -parent $yafOutMethod -label "Image Setting" -width 390 -collapsable 0 -collapse true -enable false`;}
	                attrControlGrp -attribute ($yafRenderSettingNode+".OutputFileType");
	            string $yafXML;
	            if($yafOutMethodTest==2)
				{$yafXML=`frameLayout -parent $yafOutMethod -label "XML Setting" -width 390 -collapsable 0 -collapse false -enable true`;}
				else
				{$yafXML=`frameLayout -parent $yafOutMethod -label "XML Setting" -width 390 -collapsable 0 -collapse true -enable false`;}
	                separator -parent $yafXML -height 15 -style "in";
	                text -parent $yafXML -label "here is bland till now";
	                separator -parent $yafXML -height 15 -style "in";
                    setParent ..;
                attrEnumOptionMenu -edit
                                   -changeCommand ("yOutputMethodChange"+" "+$yafRenderSettingNode+" "+$yafGUI+" "+$yafImage+" "+$yafXML)
                                   $yafOutMethodEnum;
	        separator -parent $yafOutputSetting -height 15 -style "in";
}






































