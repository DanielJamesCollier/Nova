#pragma once
#include "CacheTemplate.h"

#include <string>
#include <unordered_map>

namespace Nova
{
	class ShaderOBJ;

	class ShaderOBJCache : CacheTemplate
	{
	public:
		/*Constructors & Destructor*/
		/*--------------------------*/
		 ShaderOBJCache();
		~ShaderOBJCache();
		void Init();
		void Dispose();
		void Dispose(const std::string& filePath);
		/*--------------------------*/


		/*Getters & Setters*/
		/*--------------------------*/
		ShaderOBJ* GetShaderOBJ(const std::string& name);
		/*--------------------------*/


		/*Data Control*/
		/*--------------------------*/
		void CacheShaderOBJ(const std::string& filePath);
		void CacheShaderOBJ(const std::string& name, const std::string& filePath);
		void CacheShaderOBJ(const std::string& name, ShaderOBJ* object);
		/*--------------------------*/

		void PrintCacheContent();

	private:
		std::unordered_map<std::string /*shader file path*/, ShaderOBJ* /*object*/> m_cache;

	};
}
