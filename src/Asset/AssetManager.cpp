#include "AssetManager.h"
#include "Asset.h"
#include "MSAATexture2D.h"
#include "Texture2D.h"
#include "TextureCube.h"
//#include "Font.h"
#include "../Asset/Shader.h"
#include "../Helper/Debug.h"
#include "../Helper/Utility.h"
#include "../Math/Color.h"
#include "../Platform/Time.h"
#include "WaveFrontObjMesh.h"

namespace Theodore {
  AssetManager* AssetManager::instance = nullptr;
  AssetManager::AssetManager() {
    instance = this;
    mAssets.clear();
  }

  AssetManager::~AssetManager() { SafeContDealloc(mAssets); }

  Texture2D* AssetManager::RequestTexture(const std::string& filename, TextureFormat format, const Color& colorKey) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadImage(filename, format, colorKey)) {
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filename, TextureFormat format) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadImage(filename, format)) {
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filename, TextureFormat format, std::vector<unsigned char>& data, const Color& colorKey) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadRawTextureData(filename, format, data, colorKey)) {
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filename, TextureFormat format, std::vector<unsigned char>& data) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadRawTextureData(filename, format, data)) {
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  Texture2D* AssetManager::RequestTexture(const std::string& filename, unsigned int width, unsigned int height, TextureFormat format, unsigned char* data) {
    Texture2D* asset = static_cast<Texture2D*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new Texture2D();
      if (asset->LoadCustomTexture(width, height, format, data)) {
        asset->SetAssetName(filename);
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<Texture2D*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  TextureCube* AssetManager::RequestTexture(const CubemapRenderer* cubemap, const std::string& filename, TextureFormat format, CubemapFace face) {
    TextureCube* asset = static_cast<TextureCube*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new TextureCube();
      if (asset->LoadCubemapTexture(cubemap, filename, format, face)) {
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<TextureCube*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  MSAATexture2D* AssetManager::RequestTexture(const std::string& filename, unsigned int width, unsigned int height, TextureFormat format, unsigned int sample) {
    MSAATexture2D* asset = static_cast<MSAATexture2D*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new MSAATexture2D();
      if (asset->LoadMultiSampleTexture(width, height, format, sample)) {
        asset->SetAssetName(filename);
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] TextureFormat does not exist or Image file is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<MSAATexture2D*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      } else {
        Debug::Log(asset);
      }
    }

    return asset;
  }

  //	Font* AssetManager::RequestFont(const std::string& filename) {
  //		Font* asset = static_cast<Font*>(GetAssetByFilename(filename));
  //
  //		if (!asset) {
  //			asset = new Font();
  //			if (asset->LoadFont(filename)) {
  //				instance->StoreAsset(asset);
  //			} else {
  //				SafeDealloc(asset);
  //				return static_cast<Font*>(nullptr);
  //			}
  //		}
  //
  //		if (asset) {
  //			asset->AddReference();
  //			//Debug::Log("Font %s is loaded\n", filename.c_str());
  //		}
  //
  //		return asset;
  //	}

  Shader* AssetManager::RequestShader(const std::string& filename, ShaderType type) {
    Shader* asset = static_cast<Shader*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new Shader(type);

      File file;
      file.Open(filename, OpenMode::Read);
      if (file.IsOpen()) {
        asset->SetAssetName(file.GetBaseName());

        // start measure time
        TimePoint start = Time::GetTime();
        asset->Compile(file.ReadFile());
        instance->StoreAsset(asset);
        file.Close();
        TimePoint end = Time::GetTime();
        // end measure time

        Debug::Log("'%s' shader compile success, elapsed %fsec", file.GetBaseName().c_str(), Time::GetInterval(start, end) / 1000.f);
      } else {
        Debug::Error("'%s' file not found!", file.GetBaseName().c_str());
        SafeDealloc(asset);
        return static_cast<Shader*>(nullptr);
      }
    }

    if (asset) {
      unsigned int beforeInc = asset->mRefCount;
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Trace("'%s' exist, increase refcount %d -> %d", asset->mBaseName.c_str(), beforeInc, asset->mRefCount);
      }
    }

    return asset;
  }

  // TODO implement MeshFormat
  Mesh* AssetManager::RequestMesh(const std::string& filename, MeshFormat format) {
    WaveFrontObjMesh* asset = static_cast<WaveFrontObjMesh*>(GetAssetByBasename(filename));

    if (!asset) {
      asset = new WaveFrontObjMesh();

      if (asset->LoadObj(filename)) {
        asset->SetAssetName(filename);
        instance->StoreAsset(asset);
      } else {
        Debug::Log("Error: [%s] is not found!", filename.c_str());
        SafeDealloc(asset);
        return static_cast<Mesh*>(nullptr);
      }
    }

    if (asset) {
      asset->AddReference();
      if (asset->mRefCount != 1) {
        Debug::Log("'%s' is already loaded. so just increase reference count to %d", filename.c_str(), asset->mRefCount);
      }
    }

    return static_cast<Mesh*>(asset);
  }

  Asset* AssetManager::GetAssetByBasename(const std::string& filename) {
    std::string basename(Asset::BaseName(filename));
    for (auto asset : instance->mAssets) {
      if (asset->mBaseName == basename) {
        return asset;
      }
    }

    // none of the asset is exist.
    return nullptr;
  }

  void AssetManager::RemoveAsset(Asset* asset) {
    if (asset) {
      if (asset->mRefCount != 0) {
        asset->RemoveReference();
        if (asset->mRefCount == 0) {
          instance->mAssets.remove(asset);
          SafeDealloc(asset);
        }
      }
    }
  }

  void AssetManager::StoreAsset(Asset* asset) {
    mAssets.push_back(asset);
    asset->mIsManaged = true;
  }
}  // namespace Theodore
