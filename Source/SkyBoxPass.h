#pragma once
#include "DeferredPass.h"
#include <GLEW\glew.h>

namespace Nova
{
	class SkyBoxPass : public DeferredPass
	{
	public:
		SkyBoxPass();
		~SkyBoxPass();

		void SetCubeMapTextureUnit(unsigned int TextureUnit);

		bool Init();

		void SetMVP(const glm::mat4& mvp);

	private:
		GLuint m_cubemap;
		GLuint m_MVP;
	};


}
