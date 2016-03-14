#include "TextureCache.h"
#include "ImageLoader.h"
#include "GLTexture.h"
#include "Logger.h"
#include "GLEW\glew.h"	

namespace Nova
{

	TextureCache::~TextureCache()
	{
		Dispose();
	}

	void TextureCache::Init()
	{

	}

	void TextureCache::Dispose()
	{
		DisposeTextures();
		DisposeCubeMaps();
	}

	void TextureCache::Dispose(const std::string& texturePath)
	{
		auto search = m_cache.find(texturePath);

		if (search != m_cache.end())
		{
			glDeleteTextures(1, &search->second->id);
			m_cache.erase(search);
		}
		else
		{
			Logger::GetInstance().ErrorBlock("TextureCache Error", "Error: the texture \"" + texturePath + "\" could not be disposed as\n it was not found in the cache", true);
		}
	}

	void TextureCache::DisposeCubeMap(const std::string& name)
	{
		auto search = m_cubemapCache.find(name);

		if (search != m_cubemapCache.end())
		{
			glDeleteTextures(1, &search->second->id);
			m_cache.erase(search);
		}
		else
		{
			Logger::GetInstance().ErrorBlock("Texture Cache Error", "Error: the cubemap \"" + name + "\" could not be disposed as\n it was not found in the cache", true);
		}
	}

	void TextureCache::DisposeTextures()
	{
		for (auto i = m_cache.begin(); i != m_cache.end(); ++i)
		{
			glDeleteTextures(1, &i->second->id);
			delete i->second;
			i->second = nullptr;
		}

		DisposeCubeMaps();

		m_cache.clear();
	}

	void TextureCache::DisposeCubeMaps()
	{
		for (auto i = m_cubemapCache.begin(); i != m_cubemapCache.end(); ++i)
		{
			glDeleteTextures(1, &i->second->id);
			delete i->second;
			i->second = nullptr;
		}

		m_cubemapCache.clear();
	}

	GLTexture* TextureCache::GetTexture(const std::string& path)
	{
		auto pair_lookup = m_cache.find(path);

		// the texture is not in the cache - try and find a solution
		if (pair_lookup == m_cache.end())
		{
			GLTexture* texture = m_imageLoader.LoadTexture(path, GL_TEXTURE_2D);

			// if texture was not found in the filesystem then return the texture not found texture
			if (texture == nullptr)
			{
				Logger::GetInstance().ErrorBlock("TextureCache Error", "Error: the texture \"" + path + "\" was not found in the filesystem\nsolution: used default texture.", true);
				return GetTexture("TextureNotFound.png");
			}
			else
			{
				Logger::GetInstance().ErrorBlock("TextureCache Error", "Error: the texture \"" + path + "\" was not found in the cache\nsolution: loaded texture from the file system and added to the cache.", true);
				m_cache.emplace(path, texture);
				return texture;
			}
		}
		else
		{
			// texture was found in cache
			return pair_lookup->second;
		}
	}

	void TextureCache::CacheTexture(const std::string& path)
	{
		auto pair_lookup = m_cache.find(path);

		// the texture is not in the cache - try and find a solution
		if (pair_lookup == m_cache.end())
		{
			GLTexture* texture = m_imageLoader.LoadTexture(path, GL_TEXTURE_2D);

			// if texture was not found in the filesystem then do nothing
			if (texture == nullptr)
			{
				Logger::GetInstance().ErrorBlock("TextureCache Error", "Error: the texture \"" + path + "\" was not found in the filesystem\nsolution: no texture was added to the cache, please choose a different texture.", true);
				return;
			}
			else
			{
				m_cache.emplace(path, texture);
			}
		}
		else
		{
			Logger::GetInstance().InfoBlock("TextureCache Info", "INFO: the texture you are trying to add \"" + path + "is allready cached", true);
		}
	}

	void TextureCache::CacheTexture(const std::string& name, const std::string& path)
	{
		auto pair_lookup = m_cache.find(path);

		// the texture is not in the cache - try and find a solution
		if (pair_lookup == m_cache.end())
		{
			GLTexture* texture = m_imageLoader.LoadTexture(path, GL_TEXTURE_2D);

			// if texture was not found in the filesystem then do nothing
			if (texture == nullptr)
			{
				Logger::GetInstance().ErrorBlock("TextureCache Error", "Error: the texture \"" + path + "\" was not found in the filesystem\nsolution: no texture was added to the cache, please choose a different texture.", true);
				return;
			}
			else
			{
				m_cache.emplace(name, texture);
			}
		}
		else
		{
			Logger::GetInstance().InfoBlock("TextureCache Info", "INFO: the texture you are trying to add \"" + path + "is allready cached", true);
		}
	}

	void TextureCache::CacheTexture(const std::string& name, GLTexture* texture)
	{
		if (texture == nullptr) return;
		m_cache.emplace(name, texture);
	}

	void TextureCache::CacheCubeMap(const std::string& name, const std::string& posX, const std::string& negX, const std::string& posY, const std::string& negY, const std::string& posZ, const std::string& negZ)
	{
		GLTexture* cubemap = m_imageLoader.LoadCubeMap(posX, negX, posY, negY, posZ, negZ);

		if (cubemap == nullptr)
		{
			Logger::GetInstance().ErrorBlock("Texture Cache Error", "Name: " + name + "\nError: the cube map could not be cached as it was not loaded", true);
			return;
		}

		Logger::GetInstance().InfoBlock("Texture Cache", "Name: " + name + "\nInfo: the cube map was added to the cache", true);
		m_cubemapCache.emplace(name, cubemap);
	}

	void TextureCache::CacheCubeMap(const std::string& name, GLTexture* texture)
	{
		if (texture == nullptr || texture->id == -1 || texture->type != GL_TEXTURE_CUBE_MAP) 
		{
			Logger::GetInstance().ErrorBlock("Texture Cache Error", "Name: " + name + "\nError: the cubemap could not be added to the cache", true);
			return;
		}

		Logger::GetInstance().InfoBlock("Texture Cache", "Name: " + name + "\nInfo: the cube map was added to the cache", true);
		m_cubemapCache.emplace(name, texture);
	}

	GLTexture* TextureCache::GetCubeMap(const std::string& name)
	{
		auto search = m_cubemapCache.find(name);

		if (search == m_cubemapCache.end())
		{
			Logger::GetInstance().ErrorBlock("Texture Cache Error", "Name: " + name + "\nError: the cubemap could not be retrieved from the cache", true);
			return nullptr;
		}

		return search->second;
	}

	void TextureCache::PrintCacheInfo()
	{
		Logger::GetInstance().InfoBlock("TextureCache Info", "Cached Textures: " + std::to_string(m_cache.size()), true);
	}

	
}
