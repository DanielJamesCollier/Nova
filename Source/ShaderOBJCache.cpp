#include "ShaderOBJCache.h"
#include "ShaderOBJ.h"
#include "Logger.h"

namespace Nova
{
	ShaderOBJCache::ShaderOBJCache()
	{
		// empty
	}


	ShaderOBJCache::~ShaderOBJCache()
	{
	}

	void ShaderOBJCache::Init()
	{
		
	}

	void ShaderOBJCache::Dispose()
	{
		for (auto& obj : m_cache)
		{
			delete obj.second;
		}

		m_cache.clear();
	}

	void ShaderOBJCache::Dispose(const std::string& filePath)
	{
		auto search = m_cache.find(filePath);

		if (search == m_cache.end())
		{
			Logger::GetInstance().ErrorBlock("ShaderOBJ Cache Error", "Error: you are trying to dispose an item that is not in the cache.\nItem: " + filePath, true);
			return;
		}

		delete search->second;
		m_cache.erase(search);
	}

	ShaderOBJ* ShaderOBJCache::GetShaderOBJ(const std::string& filePath)
	{
		auto search = m_cache.find(filePath);

		if (search != m_cache.end())
		{
			return search->second;
		}
		else
		{
			// the shaderOBJ was not found in the cache - look in the file system
			ShaderOBJ* obj = new ShaderOBJ(filePath);
			
			if (obj->IsShaderLoaded())
			{
				// add only if compiled correctly
				if (obj->Compile())
				{
					m_cache.emplace(filePath, obj);
					return obj;
				}
				else
				{
					// shaderOBJ didnt compile correctly
					delete obj;
					return nullptr;
				}
			}
			else
			{
				// shaderOBJ had loading errors
				delete obj;
				return nullptr;
			}
		}
	}

	void ShaderOBJCache::CacheShaderOBJ(const std::string& filePath)
	{
		auto search = m_cache.find(filePath);

		if (search != m_cache.end())
		{
			// item allready found in the cache
			Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Name: " + filePath + "\nError: the shader object you are trying to add to the cache is allready added", true);
			return;
		}
		else
		{
			// the shaderOBJ was not found in the cache - look in the file system
			ShaderOBJ* obj = new ShaderOBJ(filePath);

			if (obj->IsShaderLoaded())
			{
				// add only if compiled correctly
				if (obj->Compile())
				{
					m_cache.emplace(filePath, obj);
				
				}
				else
				{
					// shaderOBJ didnt compile correctly
					Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Error: the shader didnt compile correctly", true);
				}
			}
			else
			{
				// shaderOBJ had loading errors
				Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Error: the shader didnt have the correct file type", true);
				
			}
		}
	}

	void ShaderOBJCache::CacheShaderOBJ(const std::string& name, const std::string& filePath)
	{
		auto search = m_cache.find(filePath);

		if (search != m_cache.end())
		{
			// item allready found in the cache
			Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Name: " + name + "\nError: the shader object you are trying to add to the cache is allready added", true);
			return;
		}
		else
		{
			// the shaderOBJ was not found in the cache - look in the file system
			ShaderOBJ* obj = new ShaderOBJ(filePath);

			if (obj->IsShaderLoaded())
			{
				// add only if compiled correctly
				if (obj->Compile())
				{
					m_cache.emplace(name, obj);
				}
				else
				{
					// shaderOBJ didnt compile correctly
					Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Error: the shader didnt compile correctly", true);
				}
			}
			else
			{
				// shaderOBJ had loading errors
				Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Error: the shader didnt have the correct file type", true);

			}
		}
	}

	void ShaderOBJCache::CacheShaderOBJ(const std::string& name, ShaderOBJ* obj)
	{
		auto search = m_cache.find(name);

		if (search != m_cache.end())
		{
			// item allready found in the cache
			Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Name: " + name + "\nError: the shader object you are trying to add to the cache is allready added", true);
			return;
		}

		if (obj == nullptr)
		{
			Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Name: " + name + "\nError: the shader object you are trying to add to the cache is null",true);
			return;
		}

		if (obj->IsShaderLoaded() && obj->Compile())
		{
			m_cache.emplace(name, obj);
			return;
		}
		
		Logger::GetInstance().ErrorBlock("ShaderOBJCache Error", "Name: " + name + "\nError: the shader was either the wrong type or failed compilation", true);
	}

	void ShaderOBJCache::PrintCacheContent()
	{

	}

}
