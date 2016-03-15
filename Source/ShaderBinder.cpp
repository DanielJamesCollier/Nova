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
	GLuint ShaderBinder::m_bindCount = 0;

	void ShaderBinder::BindShaderProgram(ShaderProgram* program)
	{
		assert(program == nullptr);

		if (m_boundShaderProgram == nullptr)
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
			m_bindCount++;
		} 
		else if (program->GetID() != m_boundShaderProgram->GetID())
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
			m_bindCount++;
		}
	}

	void ShaderBinder::UnbindShaderProgram()
	{
		if (m_boundShaderProgram == 0) return;
		m_boundShaderProgram = 0;
		glUseProgram(0);
	}

	ShaderProgram* ShaderBinder::GetCurretBoundProgram()
	{
		return m_boundShaderProgram;
	}

	GLuint ShaderBinder::GetAndResetBindCount()
	{
		GLuint bindCount = m_bindCount;
		m_bindCount = 0;
		return bindCount;
	}

}
