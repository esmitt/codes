# Multiple Render Targets

This code implements a [Multiple Render Targets](https://en.wikipedia.org/wiki/Multiple_Render_Targets) approach using C++ and OpenGL. 

For this project it is necessary the following:
* [The Image Debugger](http://www.billbaxter.com/projects/imdebug/) which is a utility to debug images in OpenGL (also Direct3D and OpenCV). The projects requires, in the same Visual Studio file location path, the files **imdebug**{.h, .lib, .dll}, **imdbgdisplay.exe** and **imdebuggl.h**.
* The [OpenGL Extension Wrangler Library](http://glew.sourceforge.net/) (GLEW) is necessary to load the shaders
* The [OpenGL Mathematics](https://glm.g-truc.net/0.9.9/index.html) (GLM) to handle the vectors and matrices in the code.
* The [FreeImage](http://freeimage.sourceforge.net/) library to handle the images
* The total environment is running using [Freeglut](http://freeglut.sourceforge.net/)

All the header files, should ne inside the _include/_ folder or the _GL/_ folder for the OpenGL-related files.

###### If you want to contribute to this project and make it better, your help is very welcome.