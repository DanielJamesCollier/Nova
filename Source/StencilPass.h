#pragma once
#include "DeferredPass.h"
#include "GLM\glm.hpp"

namespace Nova
{
	class StencilPass : public DeferredPass
	{
	public:
		StencilPass();
		~StencilPass();

		bool Init();

		void SetMVP(const glm::mat4& mvp);
		
	private:
		GLuint m_mvp;
	};


}
