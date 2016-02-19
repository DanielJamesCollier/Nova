#pragma once
#include "Material.h"
#include "CacheTemplate.h"
#include <unordered_map>
#include <string>	

namespace Nova
{
	class MaterialCache : public CacheTemplate
	{
	public:
		MaterialCache();
		~MaterialCache();

		void Init();
		void Dispose();
		void Dispose(const std::string& name);
		void AddMaterial(const std::string& name, Material* material);
		Material* GetMaterial(const std::string& name);

		void PrintCacheContent()
		{
			//todo
		}

	private:
		std::unordered_map<std::string, Material*> m_cache;
	};
	
}

