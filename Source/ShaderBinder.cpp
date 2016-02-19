#include "ShaderBinder.h"
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "Logger.h"

#include <GLEW/glew.h>
#include <iostream>
using namespace std;

namespace Nova
{

	ShaderProgram* ShaderBinder::m_boundShaderProgram = nullptr;

	void ShaderBinder::Init()
	{
		
	}

	void ShaderBinder::Dispose()
	{
		UnbindShaderProgram();
	}

	void ShaderBinder::BindShaderProgram(ShaderProgram* program)
	{
		if (m_boundShaderProgram == nullptr)
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
			program->EnableAttribArrays();
		} 
		else if (program->GetID() != m_boundShaderProgram->GetID())
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
			program->EnableAttribArrays();
		}
	}

	void ShaderBinder::BindShaderProgram(const std::string& shaderProgram)
	{
		ShaderProgram* program = ResourceManager::GetShaderProgram(shaderProgram);
		if (program == nullptr)
		{
			Logger::ErrorBlock("ShaderBinder Error", "Error: the shader program trying to be bound equals null", true);
			return;
		}

		if (m_boundShaderProgram == nullptr) // no shader is bound so bind one
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
			program->EnableAttribArrays();
		}
		else if (program->GetID() != m_boundShaderProgram->GetID()) 
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
			program->EnableAttribArrays();
		}		
	}

	void ShaderBinder::UnbindShaderProgram()
	{
		if (m_boundShaderProgram == 0) return;
		m_boundShaderProgram->DisableAttribArrays();
		m_boundShaderProgram = 0;
		glUseProgram(0);
	}

	ShaderProgram* ShaderBinder::GetCurretBoundProgram()
	{
		return m_boundShaderProgram;
	}

}
