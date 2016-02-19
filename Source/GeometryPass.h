#pragma once
#include "DeferredPass.h"
#include <GLM\glm.hpp>

namespace Nova
{
	class GeometryPass : public DeferredPass
	{
	public:
		GeometryPass();
		~GeometryPass();

		bool Init();

		
		void SetDiffuseTextureUnit(unsigned int TextureUnit);
		void SetNormalTextureUnit(unsigned int TextureUnit);
		void SetMVP(const glm::mat4& mvp);
		void SetModel(const glm::mat4& model);

	private:
	
		GLuint m_MVP;
		GLuint m_Model;
		GLuint m_diffuse;
		GLuint m_normal;
	};

}
