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

    static Texture2D* RequestTexture(const std::string& filePath, TextureFormat format, const Color& colorKey);
    static Texture2D* RequestTexture(const std::string& filePath, TextureFormat format);
    static Texture2D* RequestTexture(const std::string& filePath, TextureFormat format, std::vector<unsigned char>& data, const Color& colorKey);
    static Texture2D* RequestTexture(const std::string& filePath, TextureFormat format, std::vector<unsigned char>& data);
    static Texture2D* RequestTexture(const std::string& filePath, unsigned int width, unsigned int height, TextureFormat format, unsigned char* data);
    static TextureCube* RequestTexture(const CubemapRenderer* cubemap, const std::string& filePath, TextureFormat format, CubemapFace face);
    static MSAATexture2D* RequestTexture(const std::string& filePath, unsigned int width, unsigned int height, TextureFormat format, unsigned int sample);
    static Font* RequestFont(const std::string& filePath, unsigned int faceIndex, int height);
    static Shader* RequestShader(const std::string& filePath, ShaderType type);
    static Mesh* RequestMesh(const std::string& filePath, MeshFormat format, MeshParseOption parseOption = MeshParseOption::NONE);
    static Asset* GetAssetByFilePath(const std::string& filePath);
    static void RemoveAsset(Asset* asset);

  private:
    void StoreAsset(Asset* asset);
		static void AddReferenceThenLog(Asset* asset);
		static void LoadSuccess(const std::string& filePath, Asset* asset);

    std::list<Asset*> assets;
		static AssetManager* instance;
  };
}  // namespace Theodore

#endif /* AssetManager_h */
