#pragma once

#include <string>
#include <GLEW/glew.h>

struct ShaderOBJSource
{
	std::string				 filePath = "";
	std::string				 source = "";
	GLint                    type;
	unsigned int			 lineCount = 0;
};