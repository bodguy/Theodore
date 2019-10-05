Welcome to Theodore
=============
![https://opensource.org/licenses/Apache-2.0](license.svg)
[![Join the chat at https://gitter.im/Theodore-community/community](https://badges.gitter.im/Theodore-community/community.svg)](https://gitter.im/Theodore-community/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
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
        MeshRenderer* renderer = buda->AddComponent<MeshRenderer>();
        // set material & mesh
        renderer->SetMaterial(material);
        renderer->SetMesh(mesh);
        budda->GetComponent<Transform>()->SetScale(Vector3d(10.f, 10.f, 10.f));   
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

### clang-format

Can easily format all the .cpp, .h files recursively using clang-format-all bash script

```bash
clang-format-all src/
```