Welcome to Theodore
=============
![](license.svg)
[![Join the chat at https://gitter.im/Theodore-community/community](https://badges.gitter.im/Theodore-community/community.svg)](https://gitter.im/Theodore-community/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
### Introduction
Theodore is the open source game framework for 3D, 2D. It's goal is to make game very easy.
All of these source codes are heavily inspired by Unity system. My ECS(Entity Component System) is originally derived from game programming gems 6 but I fixed it to make codes and performances being more readable and maintainable to work well with Unity style ECS.

This is my first project that I personally called it "Everything with my own project". As you can see, the name literally means No external libraries, No Dlls for my studying purposes. I'm not very good at C++ language and even I'm also not good at OpenGL like stuffs.
but I hopefully get it better with this project.

I use only a few low level libraries which is ...
OpenGL for hardware supporting graphics pipelining.
glew for opengl version management without any considering various graphic card manufacturer.
OpenAL for embedded multithreading sound output.
stb_image for various texture format loading(PNG, JPEG, TGA, BMP etc...)
stb_freetype for truetype font loading (I prefer this than FreeType lib)

### Supporting features

+ Crossplatform support (Windows, Linux, Macos) without glfw
+ Resource management system
+ Compile and Linking Shader source code
+ Easy Transform objects with translation, rotate, scaling
+ Custom math library (Matrix, Vector, Quaternion)
+ Platform independent Input, Timer, Graphics
+ Hardware abstraction layer for multi devices only for desktop
+ Collision detection (AABB, OBB)
+ Ray casting (plane, sphere, box)
+ Component Based Object Management System
+ Gizmos for translation, rotation, scaling
+ Heirarachy transform system
+ Mathmatically generated placeholder 3d model
+ Quaternion based rotating (no gimblock)
+ Scene management system
+ Easy environment setting with CMake
+ Easy various format texture loading
+ Render To Texture with framebuffer, renderbuffer

### TODO

+ *[TODO]* Pyhsics (Verlet Integeration)
+ *[TODO]* Shadow mapping, normal mapping
+ *[TODO]* Obj, collada file reader and writer
+ *[TODO]* Skinning mesh, keyframe animation
+ *[TODO]* Water surface rendering
+ *[TODO]* Terrain integration
+ *[TODO]* Various 3d optimization technique (LOD, tesselation, frustum culling)
+ *[TODO]* PBRT
+ *[TODO]* Text rendering with unicode for multilingual support
+ *[TODO]* curve rendering (bezier, b-spline, nurbs)
+ *[TODO]* Lensflare, Bloom Filter, Defferd rendering, HDR, gamma correction
+ *[TODO]* imgui integration

### Usage

```c++
#include "Application.h"

using namespace Theodore;

int main(int argc, char** argv) {
	Application app;
	PlatformContext context;
	context.multisample = 16;

	if (app.Initialize(context)) {
		app.Run();
	}

	return 0;
}
```
