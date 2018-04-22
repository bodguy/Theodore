#include "AssetManager.h"
#include "Asset.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "MSAATexture2D.h"
//#include "Font.h"
#include "Shader.h"
#include "File.h"
#include "Color.h"
#include "Debug.h"
#include "Utility.h"

namespace Quark {
	AssetManager* AssetManager::instance = nullptr;
	AssetManager::AssetManager() {
		instance = this;
		mAssets.clear();
	}

	AssetManager::~AssetManager() {
		SafeContDealloc(mAssets);
	}

	Texture2D* AssetManager::RequestTexture(const std::string& filename, Enumeration::TextureFormat format, const Color& colorKey) {
		Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilename(filename));

		if (!asset) {
			asset = new Texture2D();
			if (asset->LoadImage(filename, format, colorKey)) {
				instance->StoreAsset(asset);
			} else {
				SafeDealloc(asset);
				return static_cast<Texture2D*>(nullptr);
			}
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
		}

		return asset;
	}

	Texture2D* AssetManager::RequestTexture(const std::string& filename, Enumeration::TextureFormat format) {
		Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilename(filename));

		if (!asset) {
			asset = new Texture2D();
			if (asset->LoadImage(filename, format)) {
				instance->StoreAsset(asset);
			} else {
				SafeDealloc(asset);
				return static_cast<Texture2D*>(nullptr);
			}
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
		}

		return asset;
	}
	
	Texture2D* AssetManager::RequestTexture(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& data, const Color& colorKey) {
		Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilename(filename));

		if (!asset) {
			asset = new Texture2D();
			if (asset->LoadRawTextureData(filename, format, data, colorKey)) {
				instance->StoreAsset(asset);
			} else {
				SafeDealloc(asset);
				return static_cast<Texture2D*>(nullptr);
			}
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
		}

		return asset;
	}

	Texture2D* AssetManager::RequestTexture(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& data) {
		Texture2D* asset = static_cast<Texture2D*>(GetAssetByFilename(filename));

		if (!asset) {
			asset = new Texture2D();
			if (asset->LoadRawTextureData(filename, format, data)) {
				instance->StoreAsset(asset);
			} else {
				SafeDealloc(asset);
				return static_cast<Texture2D*>(nullptr);
			}
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
		}

		return asset;
	}

	Texture2D* AssetManager::RequestTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned char* data) {
		Texture2D* asset = new Texture2D();
		if (asset->LoadCustomTexture(width, height, format, data)) {
			instance->StoreAsset(asset);
		} else {
			SafeDealloc(asset);
			return static_cast<Texture2D*>(nullptr);
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
		}

		return asset;
	}

	TextureCube* AssetManager::RequestTexture(unsigned int id, const std::string& filename, Enumeration::TextureFormat format, Enumeration::CubemapFace face) {
		TextureCube* asset = static_cast<TextureCube*>(GetAssetByFilename(filename));

		if (!asset) {
			asset = new TextureCube();
			if (asset->LoadCubemapTexture(id, filename, format, face)) {
				instance->StoreAsset(asset);
			} else {
				SafeDealloc(asset);
				return static_cast<TextureCube*>(nullptr);
			}
		}

		if (asset) {
			asset->AddReference();
            //Debug::Log(asset);
		}

		return asset;
	}

	MSAATexture2D* AssetManager::RequestTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned int sample) {
		MSAATexture2D* asset = new MSAATexture2D();
		if (asset->LoadMultiSampleTexture(width, height, format, sample)) {
			instance->StoreAsset(asset);
		} else {
			SafeDealloc(asset);
			return static_cast<MSAATexture2D*>(nullptr);
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
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

	Shader* AssetManager::RequestShader(const std::string& filename, Enumeration::ShaderType type) {
		Shader* asset = static_cast<Shader*>(GetAssetByFilename(filename));

		if (!asset) {
			asset = new Shader(type);

			File file;
			file.Open(filename, Enumeration::Read);
			if (file.IsOpen()) {
				asset->SetName(filename);
				asset->Compile(file.ReadUntilEnd());
				instance->StoreAsset(asset);
				file.Close();
			} else {
				SafeDealloc(asset);
				return static_cast<Shader*>(nullptr);
			}
		}

		if (asset) {
			asset->AddReference();
			//Debug::Log(asset);
		}

		return asset;
	}

	Asset* AssetManager::GetAssetByFilename(const std::string& filename) {
		for (auto i : instance->mAssets) {
			// if we find it.
			if (i->mName == filename)
				return i;
		}

		// none of the asset is exist.
		return nullptr;
	}

	void AssetManager::StoreAsset(Asset* asset) {
		mAssets.push_back(asset);
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
	} // end of function
}
