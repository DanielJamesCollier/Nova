#include "Shader.h"
#include "Logger.h"
#include "ResourceManager.h"

namespace Nova
{

	ShaderProgramCache::~ShaderProgramCache()
	{
		Dispose();
	}

	void ShaderProgramCache::Init()
	{
		// use . ini file to read in programs
	}

	void ShaderProgramCache::Dispose()
	{
		for (auto i = m_cache.begin(); i != m_cache.end(); ++i)
		{
			delete  i->second;
		}
		m_cache.clear();
	}

	void ShaderProgramCache::Dispose(const std::string& programName)
	{

		Nova::ShaderProgram* program = FindProgram(programName);

		if (program != nullptr)
		{
			delete program;
			m_cache.erase(programName);
		}
		else
		{
			Logger::ErrorBlock("Shader Program Could Not Be Deleted", "Error: the program could not be found in the program cache\nPRogram: " + programName, true);
		}
	}

	void ShaderProgramCache::CacheProgram(ShaderProgram* program)
	{
		if (program == nullptr)
		{
			Logger::ErrorBlock("ShaderProgramCache", "Name: " + program->GetName() + "\nError: the program you are trying to add to the cache is null", true);
			return;
		}
		else if (!program->IsLinked())
		{
			Logger::ErrorBlock("ShaderProgramCache", "Name: " + program->GetName() + "\nError: the program you are trying to add to the cache is not linked", true);
			return;
		}

		m_cache.emplace(program->GetName(), program);
	}

	ShaderProgram* Nova::ShaderProgramCache::GetProgram(std::string program_name)
	{
		return FindProgram(program_name);
	}

	ShaderProgram* ShaderProgramCache::FindProgram(std::string program_name)
	{
		auto search = m_cache.find(program_name);

		if (search != m_cache.end())
		{
			return search->second;
		}

		Logger::ErrorBlock("Shader Program Could Not Be Found", "Error: program not found in shader cache\nProgram: " + program_name, true);

		return nullptr;
	}

}
