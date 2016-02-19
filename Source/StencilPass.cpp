#include "StencilPass.h"
#include "ResourceManager.h"

namespace Nova
{

	StencilPass::StencilPass()
	{
	}


	StencilPass::~StencilPass()
	{
	}

	bool StencilPass::Init()
	{
		/*DIRECTION LIGHT SHADER PASS*/
		m_program = new ShaderProgram("stencilPass.glsl");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/StencilPass/StencilPass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/StencilPass/StencilPass.vs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();

		m_mvp = m_program->GetUniformLocation("MVP");

		if (m_mvp == UNIFORM_NOT_FOUND)
		{
			return false;
		}

		return DeferredPass::Init();
	}

	void StencilPass::SetMVP(const glm::mat4& mvp)
	{
		m_program->SetUniform4m(m_mvp, mvp);
	}
}
