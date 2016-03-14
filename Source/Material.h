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

		GLTexture* GetTexture()  const;
		float GetSpecularPower() const;


	private:
	
		GLTexture*  m_texture;
		float       m_specularPower;

		friend class RenderSystem;
	};

}
