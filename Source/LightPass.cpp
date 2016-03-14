#include "LightPass.h"
#include "NovaGLDefines.h"
#include "Logger.h"

namespace Nova
{
	LightPass::LightPass()
	{
	}


	LightPass::~LightPass()
	{
	}

	bool LightPass::Init()
	{
		assert(m_initialised);

		// get g buffer texture unit locations
		m_positionTextureUnit = m_program->GetUniformLocation("gBufferPosition");
		m_diffuseTextureUnit = m_program->GetUniformLocation("gBufferDiffuse");
		m_normalTextureUnit = m_program->GetUniformLocation("gBufferNormal");

		// get uniform locations
		m_MVP = m_program->GetUniformLocation("MVP");
		m_screenSize = m_program->GetUniformLocation("screenSize");
		m_specularPower = m_program->GetUniformLocation("material.matSpecularPower");
		m_eyeWorlPos = m_program->GetUniformLocation("eyeWorldPos");

		// check if uniform are found in the shader;
		if (m_positionTextureUnit == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_diffuseTextureUnit  == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_normalTextureUnit   == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_MVP                 == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_screenSize          == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_specularPower       == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_eyeWorlPos          == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND)
		{
			m_initialised = false;
			return false;
		}


		DeferredPass::Init();
	}

	void LightPass::SetMVP(const glm::mat4& MVP)
	{
		m_program->SetUniform4m(m_MVP, MVP);
	}

	void LightPass::SetPositionTextureUnit(unsigned int TextureUnit)
	{
		m_program->SetUniform1i(m_positionTextureUnit, TextureUnit);
	}

	void LightPass::SetDiffuseTextureUnit(unsigned int TextureUnit)
	{
		m_program->SetUniform1i(m_diffuseTextureUnit, TextureUnit);
	}

	void LightPass::SetNormalTextureUnit(unsigned int TextureUnit)
	{
		m_program->SetUniform1i(m_normalTextureUnit, TextureUnit);
	}

	void LightPass::SetEyeWorldPos(const glm::vec3& EyeWorldPos)
	{
		m_program->SetUniform3f(m_eyeWorlPos, EyeWorldPos);
	}

	void LightPass::SetMatSpecularPower(float Power)
	{
		m_program->SetUniform1f(m_specularPower, Power);
	}

	void LightPass::SetScreenSize(unsigned int Width, unsigned int Height)
	{
		m_program->SetUniform2f(m_screenSize, static_cast<float>(Width), static_cast<float>(Height));
	}

}

