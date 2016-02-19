#include "DeferredPass.h"

namespace Nova
{
	DeferredPass::DeferredPass()
	{
	}


	DeferredPass::~DeferredPass()
	{
	}

	void DeferredPass::Enable()
	{
		ShaderBinder::BindShaderProgram(m_program);
	}

	bool DeferredPass::IsInitialised()
	{
		return m_initialised;
	}

}

