#include "Material.h"
#include "ImageLoader.h"
#include <iostream>


namespace Nova
{
	Material::Material(const std::string& diffuse, const std::string& normal, const std::string& specular, float specularPower)
		:
		m_specularPower(specularPower)
	{
		std::vector<std::string> textures;
		textures.reserve(3);
		textures.push_back(diffuse);
		textures.push_back(normal);
		textures.push_back(specular);
		m_texture = ImageLoader::LoadImagesInto2DTexArray(textures,true);
	}

	Material::~Material()
	{
		
	}

	void Material::DisposeGLResources()
	{
		glDeleteTextures(1, &m_texture->id);
		if (m_texture != nullptr) delete m_texture;
		m_texture = nullptr;
	}

	GLTexture* Material::GetTexture() const
	{
		return m_texture;
	}

	float Material::GetSpecularPower() const
	{
		return m_specularPower;
	}
}