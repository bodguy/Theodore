// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef AssetManager_h
#define AssetManager_h

#include <list>
#include <vector>

#include "Graphics/Enumeration.h"
#include "Texture.h"

namespace Theodore {
  class Texture2D;
  class Asset;
  class Color;
  class MSAATexture2D;
  class TextureCube;
  class Shader;
  class Mesh;
  class CubemapRenderer;
  class Font;
  class AssetManager {
  public:
    AssetManager();
    ~AssetManager();

    static Texture2D* RequestTexture(const std::string& filename, TextureFormat format, const Color& colorKey);
    static Texture2D* RequestTexture(const std::string& filename, TextureFormat format);
    static Texture2D* RequestTexture(const std::string& filename, TextureFormat format, std::vector<unsigned char>& data, const Color& colorKey);
    static Texture2D* RequestTexture(const std::string& filename, TextureFormat format, std::vector<unsigned char>& data);
    static Texture2D* RequestTexture(const std::string& filename, unsigned int width, unsigned int height, TextureFormat format, unsigned char* data);
    static TextureCube* RequestTexture(const CubemapRenderer* cubemap, const std::string& filename, TextureFormat format, CubemapFace face);
    static MSAATexture2D* RequestTexture(const std::string& filename, unsigned int width, unsigned int height, TextureFormat format, unsigned int sample);
    static Font* RequestFont(const std::string& filename, unsigned int faceIndex, int height);
    static Shader* RequestShader(const std::string& filename, ShaderType type);
    static Mesh* RequestMesh(const std::string& filename, MeshFormat format = MeshFormat::WaveFrontObj);
    static Asset* GetAssetByBasename(const std::string& filename);
    static void RemoveAsset(Asset* asset);

  private:
    void StoreAsset(Asset* asset);

    static AssetManager* instance;
    std::list<Asset*> mAssets;
  };
}  // namespace Theodore

#endif /* AssetManager_h */
