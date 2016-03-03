#pragma once
#include "GLTexture.h"
#include <string>	
#include <assert.h>

namespace Nova
{
	class Material
	{

	public:
		Material() {}
		Material(const std::string& diffuse, const std::string& normal, const std::string& specular, float specularPower);
		~Material();

		void DisposeGLResources();

		void Bind(GLuint unit);

		GLuint GetID()
		{
			//assert(m_material == nullptr);
			return m_material->id;
		}

	private:
	
		GLTexture*   m_material = nullptr;
		float       m_specularPower;

		friend class RenderSystem;
	};

}
