# CSC-212-Final-Project
Left-Leaning Red-Black Trees

Authors: William Mann, Libbania Reyes, Reid Morin, Sarah Mirandou

## Dependencies
In order to compile and build this project, please download the appropriate version of [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/) for your environment. Follow the appropriate tutorial for [getting started](https://www.sfml-dev.org/tutorials/2.5/), if need be. This project was tested with the x32 version of Visual Studio 2017.

## Quick Start

Download the appropriate version of [SFML](https://www.sfml-dev.org/download/sfml/2.5.1/) for your environment. I recommend extracting the zip to a "Libraries" folder for easy access and organized storage.

Clone the repository and open "CSC-212-Final-Project.sln".

Open up your project properties in Visual Studio.

Under "C/C++" >> "General" >> "Additional Include Directories", add the path to the "include" folder in the extracted SFML folder here.

Under "Linker" >> "General" >> "Additional Library Directories", add the path to the "lib" folder in the extracted SFML folder here.

Under "Linker" >> "Input" >> "Additional Dependencies". If you are using the debug environment, add "sfml-window-s-d.lib;sfml-graphics-s-d.lib;sfml-system-s-d.lib;freetype.lib;opengl32.lib;winmm.lib;", without quotes, to the start of that line. If you are using the release environment, add "sfml-window.lib;sfml-graphics.lib;sfml-system.lib;freetype.lib;opengl32.lib;winmm.lib;", without quotes, to the start of the line.

Ensure that the environment architecture matches the architecture of the SFML you downloaded. Using a different architecture *will cause linker errors*. For example, even if you have a computer that runs on x64, if you chose the x86 SFML version, your visual studio should reflect it.

At this point, you should be clear to run the solution locally.
