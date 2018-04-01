#ifndef AssetManager_h
#define AssetManager_h

#include "Texture.h"
#include "Enumeration.h"
#include <list>
#include <vector>

namespace Quark {
	class Texture2D; class Asset; class Color; class MSAATexture2D; class TextureCube; //class Font;
	class AssetManager {
	public:
		AssetManager();
		~AssetManager();

		static Texture2D* RequestTexture(const std::string& filename, Enumeration::TextureFormat format, const Color& colorKey);
		static Texture2D* RequestTexture(const std::string& filename, Enumeration::TextureFormat format);
		static Texture2D* RequestTexture(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& data, const Color& colorKey);
		static Texture2D* RequestTexture(const std::string& filename, Enumeration::TextureFormat format, std::vector<unsigned char>& data);
		static Texture2D* RequestTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned char* data);
		static TextureCube* RequestTexture(unsigned int id, const std::string& filename, Enumeration::TextureFormat format, Enumeration::CubemapFace face);
		static MSAATexture2D* RequestTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned int sample);
		//static Font* RequestFont(const std::string& filename);
		static Asset* GetAssetByFilename(const std::string& filename);
		static void RemoveAsset(Asset* asset);

	private:
		void StoreAsset(Asset* asset);

		static AssetManager* instance;
		std::list<Asset*> mAssets;
	};
}

#endif /* AssetManager_h */
