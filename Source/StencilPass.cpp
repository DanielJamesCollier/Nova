#include "StencilPass.h"
#include "ResourceManager.h"
#include "NovaGLDefines.h"
#include <assert.h>
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
		assert(m_initialised);

		/*DIRECTION LIGHT SHADER PASS*/
		m_program = new ShaderProgram("stencilPass.glsl");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/StencilPass/StencilPass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/StencilPass/StencilPass.vs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();

		m_mvp = m_program->GetUniformLocation("MVP");

		if (m_mvp == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND)
		{
			m_initialised = false;
			return false;
		}

		return DeferredPass::Init();
	}

	void StencilPass::SetMVP(const glm::mat4& mvp)
	{
		m_program->SetUniform4m(m_mvp, mvp);
	}
}
