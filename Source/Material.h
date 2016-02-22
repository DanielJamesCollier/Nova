#pragma once
#include "GLTexture.h"
#include <string>	

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
			return m_material->id;
		}

	private:
	
		GLTexture*   m_material;
		float       m_specularPower;

		friend class RenderSystem;
	};

}
