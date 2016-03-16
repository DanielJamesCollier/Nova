#include "GeometryPass.h"
#include "ResourceManager.h"
#include "NovaGLDefines.h"

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
		std::cout << "working" << std::endl;
		if (m_program == nullptr) std::cout << "wtf" << std::endl;
		ShaderBinder::BindShaderProgram(m_program);

		
		m_MVP   = m_program->GetUniformLocation("MVP");
		m_Model = m_program->GetUniformLocation("Model");

		if (m_Model == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND || 
			m_MVP   == NOVA_SHADER_PROGRAM_UNIFORM_NOT_FOUND)
		{
			m_initialised = false;
			return m_initialised;
		}

		return DeferredPass::Init();
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
