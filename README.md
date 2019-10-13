Welcome to Theodore
=============
![https://opensource.org/licenses/Apache-2.0](license.svg)
### Introduction
Theodore is the open source game framework for 3D, 2D. It's goal is to make game very easy.
All of these source codes are heavily inspired by Unity system. My ECS(Entity Component System) is originally derived from game programming gems 6 but I fixed it to make codes and performances being more readable and maintainable to work well with Unity style ECS.

This is my first project that I personally called it "Everything with my own project". As you can see, the name literally means No external libraries, No Dlls for my studying purposes. I'm not very good at C++ language and even I'm also not good at OpenGL like stuffs.
but I hopefully get it better with this project.

I use only a few low level libraries which is:  
- OpenGL for hardware supporting GPU pipelining.  
- GLEW for OpenGL version management without any considering various graphic card manufacturer.  
- OpenAL for embedded multithreading sound output.  
- stb_image, stb_image_write for various texture format loading(PNG, JPEG, TGA, BMP etc...)
- stb_truetype for truetype(.ttf) font loading (better than FreeType library)  

### Supporting features

+ Crossplatform support (Windows, Linux, Macos) without GLFW
+ Resource management system
+ Compile and Linking Shader source code with #include preprocessor
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

### Todo

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
+ *[TODO]* broadphase, narrowphase collision detection

### Getting started

very simple example, making a engine start.

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

simple cube

```c++
class MySimpleCubeScene : public Scene {
public:
    virtual void OnAwake() {
        // make primitive using built-in function
        GameObject *cube = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
        cube->GetTransform()->SetPosition(Vector3d(0.f, 0.f, -4.f));
    }
};
```

simple point light

```c++
class MySimplePointLightScene : public Scene {
public:
    virtual void OnAwake() {
        GameObject* pointLight = new GameObject("pointLight", this);
        // add Light component
        Light* pl = pointLight->AddComponent<Light>(LightType::PointLight); 
        pl->ambient = Color::white;
        pl->diffuse = Color::white;
        pl->specular = Color::white;
        pl->GetTransform()->SetPosition(Vector3d(5.f, 0.f, 0.f));
    }
}
```

simple sprite rendering

```c++
class MySimpleSpriteRendering : public Scene {
public:
    virtual void OnAwake() {
        GameObject* sprite = new GameObject("sprite", this);
        // add SpriteRenderer component
        SpriteRenderer* rend = sprite->AddComponent<SpriteRenderer>();
        // RequestTexture args: file_path, file_format, color_key
        rend->SetSprite(Sprite::Create(AssetManager::RequestTexture(
            Application::GetResourcePath() + "sprite.png", TextureFormat::RGBA32, Color::white)));
        sprite->GetTransform()->SetLocalScale(Vector3d(0.01f, 0.01f, 0.01f));
    }
}
```

simple model loading and draw to MeshRenderer

```c++
class MySimpleMeshRendering : public Scene {
public:
    virtual void OnAwake() {
        GameObject* budda = new GameObject("budda", this);
        // RequestMesh args: file_path, mesh_format
        Mesh* mesh = AssetManager::RequestMesh(
            Application::GetResourcePath() + "model/budda.obj", MeshFormat::WaveFrontObj);
        // make a material from built-in blinn-phong shader
        Material* material = new Material(Shader::Find("Phong"));
        // add MeshRenderer component
        MeshRenderer* renderer = budda->AddComponent<MeshRenderer>();
        // set material & mesh
        renderer->SetMaterial(material);
        renderer->SetMesh(mesh);
        budda->GetComponent<Transform>()->SetLocalScale(Vector3d(10.f, 10.f, 10.f));   
    }
}
```

simple cubemap renderer, I know this code little bit weired, soon be changed.

```c++
class MySimpleCubeMapRendering : public Scene {
public:
    virtual void OnAwake() {
        GameObject* skybox = new GameObject("skybox", this);
              CubemapRenderer* cubemap = skybox->AddComponent<CubemapRenderer>();
              AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/posx.jpg", TextureFormat::RGBA32,
                                           CubemapFace::PositiveX);  // Right
              AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/negx.jpg", TextureFormat::RGBA32,
                                           CubemapFace::NegativeX);  // Left
              AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/posy.jpg", TextureFormat::RGBA32,
                                           CubemapFace::PositiveY);  // Top
              AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/negy.jpg", TextureFormat::RGBA32,
                                           CubemapFace::NegativeY);  // Bottom
              AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/posz.jpg", TextureFormat::RGBA32,
                                           CubemapFace::PositiveZ);  // Back
              AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/negz.jpg", TextureFormat::RGBA32,
                                           CubemapFace::NegativeZ);  // Front
    }
}
```

### sample

'sample' directory contains every JIT demos.  
- 01-hello_world: simple open window and bind OpenGL context
- 02-triangle: simple triangle example

### clang-format

Can easily format all the .cpp, .h files recursively using clang-format-all bash script

```bash
clang-format-all src/
```

### clang-tidy

TODO

### platform dependant IDE support

Theodore uses [cmake](https://cmake.org/download/) as a build backend. Platform specific IDEs are generated from cmake generator.
for example, when you want to build project on windows visual studio 2017:

```bash
mkdir build
cd build
cmake -G "Visual Studio 15" ..

# Note that: VS 15 is VS 2017, 
#            VS 14 is VS 2015, 
#            VS 12 is VS 2013,
#            VS 11 is VS 2012,
#            VS 10 is VS 2010
```

or macos xcode:

```bash
mkdir build
cd build
cmake -G Xcode ..
```

any linux environment, you can use makefile directly that generated from cmake.

```bash
mkdir build
cd build
cmake ..
make .
```

Jetbrain Clion IDE is good choice to build project. because they use cmake as a default build backend.  
another free IDE solution is [vscode](https://code.visualstudio.com/). vscode support cmake based C++ project with extensions.
I do not explain details here.  

Note that: minimal cmake version 3.6

### unit tests

Catch2 test framework used. every test codes are in test directory.
Theodore demands you to install Catch2.cmake script. those packages can easily be installed from [here](https://github.com/catchorg/Catch2/blob/master/docs/cmake-integration.md#installing-catch2-from-git-repository)

for the sake of brevity, the bash commands on Catch2 web resource are repeated here.

```bash
git clone https://github.com/catchorg/Catch2.git
cd Catch2
cmake -Bbuild -H. -DBUILD_TESTING=OFF
sudo cmake --build build/ --target install
```

Todo: test coverage minimum 80% than higher

### valgrind memory leak check

memory leak check tested through valgrind-3.13.0.

```bash
valgrind --tool=memcheck --gen-suppressions=all --leak-check=full --leak-resolution=med --track-origins=yes --vgdb=no Theodore
```

### common problems

when build project with gcc environment, your machine should have right compiler bundles to build project.
because of multi platform features, objc++ compiler is needed regardless of those feature use or not.
you can easily install them from package manager on debian based linux machine.

```bash
sudo apt-get install gobjc++
```

after successfully installed, cmake don't complain to you.