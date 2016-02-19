#pragma once
#ifndef SHADER_PROGRAMCACHE_HEADER
#define SHADER_PROGRAMCACHE_HEADER

#include <string>
#include <unordered_map>

namespace Nova
{
	class ShaderProgram;

	class ShaderProgramCache
	{
	public:

		/*Constructors & Destructor*/
		/*--------------------------*/
		ShaderProgramCache() = default;
		~ShaderProgramCache();
		void Init();
		void Dispose();
		void Dispose(const std::string& programName);
		/*--------------------------*/


		/*Getters & Setters*/
		/*--------------------------*/
		void		   CacheProgram(ShaderProgram* program);
		ShaderProgram* GetProgram(std::string program_name);
		/*--------------------------*/


	private:
		ShaderProgram* FindProgram(std::string program_name);

		std::unordered_map<std::string /* program name */, ShaderProgram*> m_cache;

	};
	
}
#endif //SHADER_PROGRAMCACHE_HEADER

