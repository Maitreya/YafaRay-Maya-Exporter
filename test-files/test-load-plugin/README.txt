this is a test command to load the plug-in into maya2011, and add a drop-down menu to maya's main window.

how to install on windows:

1.find the maya.env in ..\My Documents\maya\2011, open it and add this line:
  YAFARAY_PLUG_IN_PATH = f:\arioso_works\GSOC\Process\dlls\ 
  just change the path into your own folder path where all the yafaray dll files are.

2.copy what in the mel files folder into ..\My Documents\maya\2011\scripts

3.open maya, then
  1>.window->Settings/Preferences->Plug-in Manager
  2>.click "Browse" on the left of the bottom, then go to where the .mll file is
  3>.then click "load"
  4>.^o^

then you'll see a new menu appear just beside the menu "help"
click "load plug-in", then the register informations are printed into output window.