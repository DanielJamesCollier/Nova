#include "DeferredPass.h"
#include <assert.h>
namespace Nova
{
	void DeferredPass::Enable()
	{
		assert(m_program == nullptr);
		ShaderBinder::BindShaderProgram(m_program);
	}

	bool DeferredPass::IsInitialised()
	{
		return m_initialised;
	}

}

