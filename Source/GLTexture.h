#pragma once
#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>
#include <GLEW/glew.h>

namespace Nova
{
	class GLTexture
	{
	public:
		
		GLTexture(GLuint id = 0, GLuint type = 0, GLuint width = 0, GLuint height = 0, GLuint components = 0)
		{
			this->id = id;
			this->type = type;
			this->width = width;
			this->height = height;
			this->components = components;
		}

		GLuint id;
		GLuint type;
		GLint  width;
		GLint  height;
		GLint  components;

	};
}


#endif // TEXTURE_HEADER

