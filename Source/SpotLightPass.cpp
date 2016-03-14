#include "SpotLightPass.h"
#include "ResourceManager.h"
#include "NovaGLDefines.h"
#include "Logger.h"
namespace Nova
{
	SpotLightPass::SpotLightPass()
	{
	}

	SpotLightPass::~SpotLightPass()
	{
	}

	bool SpotLightPass::Init()
	{
		assert(m_initialised);

		m_program = new ShaderProgram("spotLightPass.glsl");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/SpotLightPass/SpotLightPass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/LightingPass/Common/deferredLightingPass.vs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();


		//get uniforms locations for point light
		m_spotLightLocation.Color = m_program->GetUniformLocation("sLight.base.base.colour");
		m_spotLightLocation.AmbientIntensity = m_program->GetUniformLocation("sLight.base.base.ambientIntensity");
		m_spotLightLocation.Position = m_program->GetUniformLocation("sLight.base.position");
		m_spotLightLocation.DiffuseIntensity = m_program->GetUniformLocation("sLight.base.base.diffuseIntensity");
		m_spotLightLocation.Direction = m_program->GetUniformLocation("sLight.direction");
		m_spotLightLocation.Cutoff = m_program->GetUniformLocation("sLight.cutoff");
		m_spotLightLocation.Atten.Constant = m_program->GetUniformLocation("sLight.base.atten.constant");
		m_spotLightLocation.Atten.Linear = m_program->GetUniformLocation("sLight.base.atten.linear");
		m_spotLightLocation.Atten.Exp = m_program->GetUniformLocation("sLight.base.atten.exp");
		

		// check if uniform are found in the shader;
		if (m_spotLightLocation.Color             == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.AmbientIntensity  == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.Position          == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.DiffuseIntensity  == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.Direction         == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.Cutoff            == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.Atten.Constant    == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.Atten.Linear      == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND ||
			m_spotLightLocation.Atten.Exp         == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND)
		{
			m_initialised = false;
			return false;
		}

		return LightPass::Init();
	}

	void SpotLightPass::SetSpotLight(const SpotLight& sLight)
	{
		glUniform3f(m_spotLightLocation.Color, sLight.base.base.colour.x, sLight.base.base.colour.y, sLight.base.base.colour.z);
		glUniform1f(m_spotLightLocation.AmbientIntensity, sLight.base.base.ambientIntensity);
		glUniform1f(m_spotLightLocation.DiffuseIntensity, sLight.base.base.diffuseIntensity);
		glUniform3f(m_spotLightLocation.Direction, sLight.direction.x, sLight.direction.y, sLight.direction.z);
		glUniform1f(m_spotLightLocation.Cutoff, sLight.cutoff);
		glUniform3f(m_spotLightLocation.Position, sLight.base.position.x, sLight.base.position.y, sLight.base.position.z);
		glUniform1f(m_spotLightLocation.Atten.Constant, sLight.base.atten.constant);
		glUniform1f(m_spotLightLocation.Atten.Linear, sLight.base.atten.linear);
		glUniform1f(m_spotLightLocation.Atten.Exp, sLight.base.atten.exp);
	}

}
