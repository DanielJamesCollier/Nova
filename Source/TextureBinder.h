#pragma once
#include <GLEW\glew.h>
#include "GLTexture.h"
#include <vector>

namespace Nova
{
	class TextureBinder
	{
		GLuint                               m_activeUnit;
		GLint                                m_maxTextureUnits;
		GLint								 m_maxTextureTargets;
		std::vector<std::vector<GLTexture*>> m_boundTextures;

		GLuint m_bindCount;

	public:

		static TextureBinder& GetInstance()
		{
			static TextureBinder binder;
			return binder;
		}

		TextureBinder(TextureBinder const&) = delete;
		void operator=(TextureBinder&) = delete;

		void BindTexture(GLuint unit, GLTexture* texture);

		GLuint GetAndResetBindCount();
		GLuint GetBindCount() const;

	private:
		TextureBinder();
		GLuint GetActiveTextureUnit() const;
		GLuint ConvertIndexToTextureType(GLuint index) const;
		GLuint ConvertTextureTargetToIndex(GLuint target) const;

		void Temp(GLuint unit, GLTexture* texture);
	};
}