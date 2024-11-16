# Godot-AdlibAudio
A Godot module that uses Adplug to emulate OPL2 music.

## Dependecies
Adplug v2.3.3 and Libbinio v1.5, Those are the only versions it's been tested with, but you can try later ones.
make sure to compile the Adplug and Libbinio dependecies for the architecture you're compiling godot, if it's x86_64, make sure the libraries are also x86_64.
Also this is only tested on x86_64 windows.

## Instructions
First off you can place the svg file in editor/icons of your custom compiled Godot folder, so there will be an icon for the AudioStreamAdlib class.
download the "adplug" folder from this repo and place it in modules.
in "modules/adplug/" place your adplug folder and your libbinio folder.
You can also look at the SCsub file for more details on where the libraries and include files should be placed exactly.

## Important
Finally, you will have to manually go into the "adplug_loader.cpp", in the get_recognized_extensions function, comment out the extensions you aren't gonna use and comment back in the ones you are gonna use. because unfortanetly godot limits the number of recognized extensions to a certain amount (I'm not sure what exactly).

## Known issues
Dual Opl2 doesn't work yet, at all.
