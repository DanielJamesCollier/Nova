#pragma once
#ifndef TEXTURE_CACHE_HEADER
#define TEXTURE_CACHE_HEADER

#include "ImageLoader.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace Nova
{
	class GLTexture;

	class TextureCache
	{
	public:


		/*Constructors & Destructor*/
		/*--------------------------*/
		TextureCache() = default;
		~TextureCache();
		void Init();
		void Dispose();
		void Dispose(const std::string& texturePath);
		void DisposeCubeMap(const std::string& name);
		void DisposeTextures();
		void DisposeCubeMaps();
		/*--------------------------*/

		/*Getters*/
		/*--------------------------*/
		GLTexture* GetTexture(const std::string& path);
		void	   CacheTexture(const std::string& path);
		void	   CacheTexture(const std::string& name, const std::string& path);
		void	   CacheTexture(const std::string& name, GLTexture* texture);
		void	   CacheCubeMap(const std::string& name, const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ);
		void	   CacheCubeMap(const std::string& name, GLTexture* texture);
		GLTexture* GetCubeMap(const std::string& name);
	
		/*--------------------------*/


		/*Texture Arrays*/
		/*--------------------------*/
		GLTexture* Create2DTextureArray(const std::string& arrayName,const std::vector<std::string>& texturePaths, int maxWidth, int maxHeight, int layerCount);
		/*--------------------------*/

		void PrintCacheInfo();

	private:

		std::unordered_map<std::string, GLTexture*> m_cache;
		std::unordered_map<std::string, GLTexture*> m_cubemapCache;
		ImageLoader									m_imageLoader;
	};
}
#endif //TEXTURE_CACHE_HEADER

