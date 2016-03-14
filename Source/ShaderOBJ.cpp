#include "ShaderOBJ.h"
#include "ShaderParser.h"
#include "ShaderError.h"


/*
- correct line number error handeling - #include breaks this
*/

namespace Nova
{
	ShaderOBJ::ShaderOBJ(const std::string& filePath)
	{
		ShaderParser::LoadShaderOBJSource(filePath, m_source);
	}


	ShaderOBJ::~ShaderOBJ()
	{
		Dispose();
	}

	void ShaderOBJ::Dispose()
	{
		if (m_disposed) return;
		m_disposed = true;
		m_compiled = false;

		glDeleteShader(m_ID);

		m_source.filePath = "";
		m_source.lineCount = 0;
		m_source.type = -1;
		m_source.source = "";
	}

	bool ShaderOBJ::IsShaderLoaded()
	{
		if (m_source.type != -1) return true;
		return false;
	}

	bool ShaderOBJ::IsShaderCompiled()
	{
		return m_compiled;
	}

	bool ShaderOBJ::Compile()
	{
		if (m_compiled || m_source.type == -1) return false;
		m_disposed = false;

		m_ID = glCreateShader(m_source.type);

		const GLchar* shaderSourceStrings[1];
		GLint shaderSourceStringLengths[1];

		shaderSourceStrings[0] = m_source.source.c_str();
		shaderSourceStringLengths[0] = m_source.source.length();

		glShaderSource(m_ID, 1, shaderSourceStrings, shaderSourceStringLengths);
		glCompileShader(m_ID);

		// check for a sucessful compile
		if (Nova::IsShaderOkay(m_ID, GL_COMPILE_STATUS, false, "ShaderOBJ: " + m_source.filePath + " - Error:	Failed to Compile"))
		{
			m_compiled = true;
			return true;
		}
		//m_compiled = false;
		return false;
	}

	const std::string& ShaderOBJ::GetSource()
	{
		return m_source.source;
	}

	const std::string& ShaderOBJ::GetName()
	{
		return m_source.filePath;
	}

	GLint ShaderOBJ::GetID()
	{
		return m_ID;
	}
}
