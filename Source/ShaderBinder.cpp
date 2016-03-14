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

	void ShaderBinder::BindShaderProgram(ShaderProgram* program)
	{
		assert(program == nullptr);

		if (m_boundShaderProgram == nullptr)
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
		} 
		else if (program->GetID() != m_boundShaderProgram->GetID())
		{
			m_boundShaderProgram = program;
			glUseProgram(program->GetID());
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

}
