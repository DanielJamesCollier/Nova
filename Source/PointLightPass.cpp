#include "PointLightPass.h"
#include "ResourceManager.h"
#include <iostream>

namespace Nova
{
	PointLightPass::PointLightPass()
	{
	}


	PointLightPass::~PointLightPass()
	{
	}

	bool PointLightPass::Init()
	{
		m_program = new ShaderProgram("pointLightPass.glsl");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/PointLightPass/DeferredPointLightPass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/Common/DeferredLightingPass.vs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();

		//get uniforms locations for point light
		m_pointLightLocation.Color = m_program->GetUniformLocation("pLight.base.colour");
		m_pointLightLocation.AmbientIntensity = m_program->GetUniformLocation("pLight.base.ambientIntensity");
		m_pointLightLocation.Position = m_program->GetUniformLocation("pLight.position");
		m_pointLightLocation.DiffuseIntensity = m_program->GetUniformLocation("pLight.base.diffuseIntensity");
		m_pointLightLocation.Atten.Constant = m_program->GetUniformLocation("pLight.atten.constant");
		m_pointLightLocation.Atten.Linear = m_program->GetUniformLocation("pLight.atten.linear");
		m_pointLightLocation.Atten.Exp = m_program->GetUniformLocation("pLight.atten.exp");

		// check if uniform are found in the shader;
		if (m_pointLightLocation.Color == UNIFORM_NOT_FOUND ||
			m_pointLightLocation.AmbientIntensity == UNIFORM_NOT_FOUND ||
			m_pointLightLocation.Position == UNIFORM_NOT_FOUND ||
			m_pointLightLocation.DiffuseIntensity == UNIFORM_NOT_FOUND ||
			m_pointLightLocation.Atten.Constant == UNIFORM_NOT_FOUND ||
			m_pointLightLocation.Atten.Linear == UNIFORM_NOT_FOUND ||
			m_pointLightLocation.Atten.Exp == UNIFORM_NOT_FOUND)
		{
			return false;
		}

		return LightPass::Init();
	}

	void PointLightPass::SetPointLight(const PointLight& pLight)
	{
		glUniform3f(m_pointLightLocation.Color, pLight.base.colour.x, pLight.base.colour.y, pLight.base.colour.z);
		glUniform1f(m_pointLightLocation.AmbientIntensity, pLight.base.ambientIntensity);
		glUniform1f(m_pointLightLocation.DiffuseIntensity, pLight.base.diffuseIntensity);
		glUniform3f(m_pointLightLocation.Position, pLight.position.x, pLight.position.y, pLight.position.z);
		glUniform1f(m_pointLightLocation.Atten.Constant, pLight.atten.constant);
		glUniform1f(m_pointLightLocation.Atten.Linear, pLight.atten.linear);
		glUniform1f(m_pointLightLocation.Atten.Exp, pLight.atten.exp);
	}

}

