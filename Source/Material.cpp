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
		m_material = ImageLoader::LoadImagesInto2DTexArray(textures,true);
	}

	Material::~Material()
	{
		
	}

	void Material::DisposeGLResources()
	{
		glDeleteTextures(1, &m_material->id);
		if (m_material != nullptr) delete m_material;
		m_material = nullptr;
	}

	void Material::Bind(GLuint unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_material->id);
	}
}