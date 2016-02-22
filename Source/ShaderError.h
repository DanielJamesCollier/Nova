#pragma once
#include "Logger.h"
#include <GLEW/glew.h>
#include <string>

namespace Nova
{
	static bool IsShaderOkay(int shader, int flag, bool isProgram, const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		if (isProgram) glGetProgramiv(shader, flag, &success);
		else           glGetShaderiv(shader, flag, &success);

		if (!success)
		{
			if (isProgram) glGetProgramInfoLog(shader, sizeof(error), NULL, error);
			else           glGetShaderInfoLog(shader, sizeof(error), NULL, error);

			Logger::GetInstance().ErrorBlock(errorMessage, error, true);
			return false;
		}

		return true;
	}
}
