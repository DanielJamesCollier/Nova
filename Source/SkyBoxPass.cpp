#include "SkyBoxPass.h"
#include "ResourceManager.h"

namespace Nova
{
	SkyBoxPass::SkyBoxPass()
	{
	}


	SkyBoxPass::~SkyBoxPass()
	{
	}

	bool SkyBoxPass::Init()
	{
		m_program = new ShaderProgram("skybox.prog");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/GeometryPass/SkyboxPass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/GeometryPass/SkyboxPass.vs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();

		m_cubemap = m_program->GetUniformLocation("cubemap");
		m_MVP = m_program->GetUniformLocation("MVP");

		return DeferredPass::Init();
	}

	void SkyBoxPass::SetCubeMapTextureUnit(unsigned int TextureUnit)
	{
		m_program->SetUniform1i(m_cubemap, TextureUnit);
	}

	void SkyBoxPass::SetMVP(const glm::mat4& mvp)
	{
		m_program->SetUniform4m(m_MVP, mvp);
	}

}
