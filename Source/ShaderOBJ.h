#pragma once
#include "ShaderOBJSource.h"
#include "ShaderOBJType.h"
#include <GLEW/glew.h>
#include <string>

namespace Nova
{
	class ShaderOBJ
	{
	public:

		explicit ShaderOBJ(const std::string& filePath);
		~ShaderOBJ();
		void Dispose();

		bool IsShaderLoaded();
		bool IsShaderCompiled();

		bool Compile();



		const std::string& GetSource();
		const std::string& GetName();
		GLint              GetID();

	private:
		ShaderOBJSource     m_source;
		GLint               m_ID = -1;
		bool				m_compiled = false;
		bool				m_disposed = false;
	};
}

