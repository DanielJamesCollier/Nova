#include "MaterialCache.h"
#include "Logger.h"

namespace Nova
{
	MaterialCache::MaterialCache()
	{
	}

	MaterialCache::~MaterialCache()
	{
		Dispose();
	}

	void MaterialCache::Init()
	{

	}

	void MaterialCache::Dispose()
	{
		for (auto i = m_cache.begin(); i != m_cache.end(); i++)
		{
			i->second->DisposeGLResources();
			delete i->second;
		}

		m_cache.clear();
	}

	void MaterialCache::Dispose(const std::string& name)
	{
		auto search = m_cache.find(name);

		if (search != m_cache.end())
		{
			delete search->second;
			m_cache.erase(search);
			return;
		}

		Logger::GetInstance().ErrorBlock("Material Cache Error", "Name: " + name + "\nError: the material you where trying to dispose was not found", true);
	}

	void MaterialCache::AddMaterial(const std::string& name, Material* material)
	{
		if (material != nullptr) 
		{
			m_cache.emplace(name, material);
			return;
		}
		
		Logger::GetInstance().ErrorBlock("Material Cache Error", "Name: " + name + "\nError: the material equals nullptr",true);
	}

	Material* MaterialCache::GetMaterial(const std::string& name)
	{
		auto search = m_cache.find(name);

		if (search != m_cache.end())
		{
			return search->second;
		}

		Logger::GetInstance().ErrorBlock("Material cache Error", "Name: " + name + "\nError: the material was not found in the cache", true);
		return nullptr;
	}
}
