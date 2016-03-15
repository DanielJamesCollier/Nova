#pragma once
#include "TextureBinder.h"
#include "Logger.h"
#include <assert.h>
#include <iostream>
namespace Nova
{
	/* zero is the default opengl active texture unit
	- glActiveTexture(unit) only needs to be called for multitexturing

	Texture Targets
		0  - GL_TEXTURE_1D
		1  - GL_TEXTURE_2D
		2  - GL_TEXTURE_3D
		3  - GL_TEXTURE_1D_ARRAY
		4  - GL_TEXTURE_2D_ARRAY
		5  - GL_TEXTURE_RECTANGLE
		6  - GL_TEXTURE_CUBE_MAP
		7  - GL_TEXTURE_CUBE_MAP_ARRAY
		6  - GL_TEXTURE_BUFFER
		9  - GL_TEXTURE_2D_MULTISAMPLE
		10 - GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		//   11 total
	*/

	TextureBinder::TextureBinder()
		:
		m_activeUnit(0),
		m_maxTextureTargets(11)
	{
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureUnits);

		std::cout << "MAX_TEXTURE_UNITS: " << m_maxTextureUnits << std::endl;

		// set the size of the array
		m_boundTextures.resize(m_maxTextureTargets);
		for (auto& x : m_boundTextures) x.resize(m_maxTextureUnits);

		glActiveTexture(GL_TEXTURE0);
	}

	void TextureBinder::BindTexture(GLuint unit, GLTexture* texture)
	{
		assert(unit > m_maxTextureUnits || texture == nullptr);

		GLuint targetIndex = ConvertTextureTargetToIndex(texture->type);

		if (m_boundTextures.at(targetIndex).at(unit) == nullptr)
		{
			m_boundTextures.at(targetIndex).at(unit) == texture;

			m_bindCount++;

			if (m_activeUnit != unit)
			{
				glActiveTexture(GL_TEXTURE0 + unit);
				m_activeUnit = unit;
			}
			
			glBindTexture(texture->type, texture->id);
			return;
		}

		if (m_boundTextures.at(targetIndex).at(unit)->id != texture->id)
		{
			m_boundTextures.at(targetIndex).at(unit) == texture;

			m_bindCount++;

			if (m_activeUnit != unit)
			{
				glActiveTexture(GL_TEXTURE0 + unit);
				m_activeUnit = unit;
			}

			glBindTexture(texture->type, texture->id);
			return;
		}
	}

	GLuint TextureBinder::GetActiveTextureUnit() const
	{
		return m_activeUnit;
	}

	GLuint TextureBinder::ConvertIndexToTextureType(GLuint index) const
	{
		switch (index)
		{
		case 0:
			return GL_TEXTURE_1D;
		case 1:
			return GL_TEXTURE_2D;
		case 2:
			return GL_TEXTURE_3D;
		case 3:
			return GL_TEXTURE_1D_ARRAY;
		case 4:
			return GL_TEXTURE_2D_ARRAY;
		case 5:
			return GL_TEXTURE_RECTANGLE;
		case 6:
			return GL_TEXTURE_CUBE_MAP;
		case 7:
			return GL_TEXTURE_CUBE_MAP_ARRAY;
		case 8: 
			return GL_TEXTURE_BUFFER;
		case 9:
			return GL_TEXTURE_2D_MULTISAMPLE;
		case 10:
			return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		default:
			Logger::GetInstance().ErrorBlock("TextureBinderError", "Error: invalid index", true);
			assert();
			return GL_INVALID_OPERATION; // to change
		}
	}

	GLuint TextureBinder::ConvertTextureTargetToIndex(GLuint target) const
	{
		switch (target)
		{
		case GL_TEXTURE_1D:
			return 0;
		case GL_TEXTURE_2D:
			return 1;
		case GL_TEXTURE_3D:
			return 2;
		case GL_TEXTURE_1D_ARRAY:
			return 3;
		case GL_TEXTURE_2D_ARRAY:
			return 4;
		case GL_TEXTURE_RECTANGLE:
			return 5;
		case GL_TEXTURE_CUBE_MAP:
			return 6;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			return 7;
		case GL_TEXTURE_BUFFER:
			return 8;
		case GL_TEXTURE_2D_MULTISAMPLE:
			return 9;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			return 10;
		default:
			fault:
				Logger::GetInstance().ErrorBlock("TextureBinderError", "Error: invalid target", true);
				assert();
			return GL_INVALID_OPERATION; // to change
		}
	}

	GLuint TextureBinder::GetAndResetBindCount()
	{
		GLuint bpf = m_bindCount;
		m_bindCount = 0;
		return bpf;
	}

	GLuint TextureBinder::GetBindCount() const
	{
		return m_bindCount;
	}
}