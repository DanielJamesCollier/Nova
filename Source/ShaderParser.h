#pragma once

#include "ShaderOBJSource.h"

#include <string>
#include <map>
#include <vector>
#include <GLEW/glew.h>
#include <set>

namespace Nova
{
	
	class ShaderParser
	{
	public:
		 ShaderParser();
		~ShaderParser();

		static void LoadShaderOBJSource(const std::string& filePath, ShaderOBJSource& shaderSource);

	private:

		static void Include(const std::string& filePath, ShaderOBJSource& shaderSource, std::set<std::string>& includes);
	};
}
