// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "AssetManager.h"
#include "Asset.h"
#include "Font.h"
#include "Helper/Debug.h"
#include "Helper/Utility.h"
#include "Helper/File.h"
#include "MSAATexture2D.h"
#include "Platform/Time.h"
#include "Shader.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "WaveFrontObjMesh.h"

namespace Theodore {
  AssetManager* AssetManager::instance = nullptr;
  AssetManager::AssetManager() {
    instance = this;
    assets.clear();
  }

  AssetManager::~AssetManager() { SafeContDealloc(assets); }

  Texture2D* AssetManager::RequestTexture(const std::string& filePath, TextureFormat format, const Color& colorKey) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadImage(filePath, format, colorKey)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filePath, TextureFormat format) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadImage(filePath, format)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filePath, TextureFormat format, std::vector<unsigned char>& data, const Color& colorKey) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadRawTextureData(filePath, format, data, colorKey)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filePath, TextureFormat format, std::vector<unsigned char>& data) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadRawTextureData(filePath, format, data)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filePath, unsigned int width, unsigned int height, TextureFormat format, unsigned char* data) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadCustomTexture(width, height, format, data)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  TextureCube* AssetManager::RequestTexture(const CubemapRenderer* cubemap, const std::string& filePath, TextureFormat format, CubemapFace face) {
    TextureCube* asset = static_cast<TextureCube*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new TextureCube();
      if (asset->LoadCubemapTexture(cubemap, filePath, format, face)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<TextureCube*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  MSAATexture2D* AssetManager::RequestTexture(const std::string& filePath, unsigned int width, unsigned int height, TextureFormat format, unsigned int sample) {
    MSAATexture2D* asset = static_cast<MSAATexture2D*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new MSAATexture2D();
      if (asset->LoadMultiSampleTexture(width, height, format, sample)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] TextureFormat does not exist or Image file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<MSAATexture2D*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Font* AssetManager::RequestFont(const std::string& filePath, unsigned int faceIndex, int height) {
    Font* asset = static_cast<Font*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Font();
      if (asset->InitFont(filePath, faceIndex, height)) {
        LoadSuccess(filePath, asset);
//        asset->LoadGlyph("A");
//        asset->LoadGlyph("B");
//        asset->LoadGlyph("C");
//        asset->LoadGlyph("D");
//        asset->LoadGlyph("E");
//        asset->LoadGlyph("F");
//        asset->LoadGlyph("G");

      } else {
        Debug::Error("[%s] font file is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Font*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Shader* AssetManager::RequestShader(const std::string& filePath, ShaderType type) {
    Shader* asset = static_cast<Shader*>(GetAssetByFilePath(filePath));

    if (!asset) {
      asset = new Shader(type);

      File file(filePath, OpenMode::Read);
      if (file.IsOpen()) {
        asset->SetAssetName(file.GetBaseName());

        // start measure time
        TimePoint start = Time::GetTime();
        int result = asset->Compile(file.ReadFile());
        // compile error
        if (!result) {
          Debug::Error("[%s] compilation error", file.GetBaseName().c_str());
          SafeDealloc(asset);
          file.Close();
          return static_cast<Shader*>(nullptr);
        }
        instance->StoreAsset(asset);
        file.Close();
        TimePoint end = Time::GetTime();
        // end measure time

        Debug::Log("[%s] compilation success, elapsed %fsec", file.GetBaseName().c_str(), Time::GetInterval(start, end) / 1000.f);
      } else {
        Debug::Error("[%s] file not found!", file.GetBaseName().c_str());
        SafeDealloc(asset);
        return static_cast<Shader*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return asset;
  }

  Mesh* AssetManager::RequestMesh(const std::string& filePath, MeshFormat format, MeshParseOption parseOption) {
    Mesh* asset = static_cast<Mesh*>(GetAssetByFilePath(filePath));

    if (!asset) {
      switch (format) {
        case MeshFormat::WaveFrontObj:
          asset = new WaveFrontObjMesh();
          break;
        default:
          Debug::Error("Unknown Meshformat");
          break;
      }

      if (asset->LoadMesh(filePath, parseOption)) {
        LoadSuccess(filePath, asset);
      } else {
        Debug::Error("[%s] is not found!", filePath.c_str());
        SafeDealloc(asset);
        return static_cast<Mesh*>(nullptr);
      }
    }

		AddReferenceThenLog(asset);
    return static_cast<Mesh*>(asset);
  }

  Asset* AssetManager::GetAssetByFilePath(const std::string& filePath) {
    for (auto asset : instance->assets) {
      if (asset->filePath == filePath) {
        return asset;
      }
    }

    // none of the asset is exist.
    return nullptr;
  }

  void AssetManager::RemoveAsset(Asset* asset) {
    if (!asset || asset->referenceCount == 0) return;

		asset->RemoveReference();
		if (asset->referenceCount == 0) {
			instance->assets.remove(asset);
			SafeDealloc(asset);
		}
  }

  void AssetManager::StoreAsset(Asset* asset) {
    assets.push_back(asset);
    asset->isManaged = true;
  }

	void AssetManager::AddReferenceThenLog(Asset* asset) {
		if (!asset) return;

		unsigned int beforeRefCount = asset->referenceCount;
		asset->AddReference();
		if (asset->referenceCount != 1) {
			Debug::Trace("[%s] is already exist, increase refcount %d -> %d", asset->fileName.c_str(), beforeRefCount, asset->referenceCount);
			return;
		}

		Debug::Log("[%s] is successfully loaded", asset->fileName.c_str());
	}

  void AssetManager::LoadSuccess(const std::string& filePath, Asset* asset) {
    asset->SetAssetName(filePath);
    instance->StoreAsset(asset);
  }
}  // namespace Theodore
