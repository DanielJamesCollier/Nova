#include "GeometryPass.h"
#include "ResourceManager.h"

namespace Nova
{
	GeometryPass::GeometryPass()
	{
	}


	GeometryPass::~GeometryPass()
	{
	}

	bool GeometryPass::Init()
	{
		m_program = new ShaderProgram("deferredGeometryPass.glsl");
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/GeometryPass/deferred_geometry_pass.fs"));
		m_program->AddShaderObject(ResourceManager::GetShaderOBJ("Shaders/Deferred/GeometryPass/deferred_geometry_pass.vs"));
		m_program->LinkProgram();
		ResourceManager::CacheShaderProgram(m_program);
		m_program->Bind();

		
		m_diffuse = m_program->GetUniformLocation("texture0");
		m_normal  = m_program->GetUniformLocation("normalMap");
		m_MVP = m_program->GetUniformLocation("MVP");
		m_Model = m_program->GetUniformLocation("Model");

		// check if uniform are found in the shader;
		if (m_diffuse == UNIFORM_NOT_FOUND   ||
			m_normal  == UNIFORM_NOT_FOUND)
		{
			return false;
		}

		return DeferredPass::Init();
	}

	void GeometryPass::SetDiffuseTextureUnit(unsigned int TextureUnit)
	{
		m_program->SetUniform1i(m_diffuse, TextureUnit);
	}

	void GeometryPass::SetNormalTextureUnit(unsigned int TextureUnit)
	{
		m_program->SetUniform1i(m_normal, TextureUnit);
	}

	void GeometryPass::SetMVP(const glm::mat4& mvp)
	{
		m_program->SetUniform4m(m_MVP, mvp);
	}

	void GeometryPass::SetModel(const glm::mat4& model)
	{
		m_program->SetUniform4m(m_Model, model);
	}

}
