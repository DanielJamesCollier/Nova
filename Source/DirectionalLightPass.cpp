#include "DirectionalLightPass.h"
#include "ResourceManager.h"

namespace Nova
{
	DirectionalLightPass::DirectionalLightPass()
	{
	}


	DirectionalLightPass::~DirectionalLightPass()
	{
	}

	bool DirectionalLightPass::Init()
	{
		/* DIRECTION LIGHT SHADER PASS*/
		m_program = new ShaderProgram("directionalLightPass.glsl");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/DirectionalLightPass/DeferredDirectionLightPass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/DirectionalLightPass/DeferredDirectionLightPass.vs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/DirectionalLightPass/DeferredDirectionLightPass.gs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();

		m_dirLightLocation.Color			= m_program->GetUniformLocation("dLight.base.colour");
		m_dirLightLocation.AmbientIntensity = m_program->GetUniformLocation("dLight.base.ambientIntensity");
		m_dirLightLocation.Direction		= m_program->GetUniformLocation("dLight.direction");
		m_dirLightLocation.DiffuseIntensity = m_program->GetUniformLocation("dLight.base.diffuseIntensity");

		// check if uniform are found in the shader;
		if (m_dirLightLocation.Color == UNIFORM_NOT_FOUND ||
			m_dirLightLocation.AmbientIntensity == UNIFORM_NOT_FOUND ||
			m_dirLightLocation.Direction == UNIFORM_NOT_FOUND ||
			m_dirLightLocation.DiffuseIntensity == UNIFORM_NOT_FOUND)
		{
			return false;
		}

		return LightPass::Init();
	}

	void DirectionalLightPass::SetDirectionalLight(const DirectionalLight& dLight)
	{
		glUniform3f(m_dirLightLocation.Color, dLight.base.colour.x, dLight.base.colour.y, dLight.base.colour.z);
		glUniform1f(m_dirLightLocation.AmbientIntensity, dLight.base.ambientIntensity);
		glm::vec3 direction = dLight.direction;
		glm::normalize(direction);
		glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
		glUniform1f(m_dirLightLocation.DiffuseIntensity, dLight.base.diffuseIntensity);
	}

}
