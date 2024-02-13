This project is to simulate the collision in 3D space.
In this project, I use a custom class, threading, and OpenGL to load and display animated 3D objects. 
The program loads and renders the objects in the 3D screen. 
The animated 3D objects are frozen in place until the user presses the “g” key 
and then the objects start moving around at random speeds and rotating randomly about an axis. 
The objects are able to collide and bounce off each other. 
The objects are also confined to the space around the center of the scene and cannot just float off into space.


All files can copy and paste to the OpenGL standard code, ogl-2.1_branch, from GitHub.

The files revised and added for this project includes CMakeLists.txt, controls.cpp, spooky.bmp, StandardShading.fragmentshader, and tutorial09_several_objects.cpp.
Those files should be at the following paths before compiling and running the program.

/ogl-2.1_branch/CMakeLists.txt
/ogl-2.1_branch/common/controls.cpp
/ogl-2.1_branch/tutorial09_vbo_indexing/spooky.bmp
/ogl-2.1_branch/tutorial09_vbo_indexing/StandardShading.fragmentshader
/ogl-2.1_branch/tutorial09_vbo_indexing/tutorial09_several_objects.cpp

The program is compiled and run based on the following environment:

> gcc --version
Apple clang version 14.0.3 (clang-1403.0.22.14.1)
Target: arm64-apple-darwin22.4.0
Thread model: posix

> Operating System: macOS Ventura 13.3
> Chip: Apple M2 Pro
> Memory: 16GB
