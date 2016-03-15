#pragma once
#include <string>
#include <GLEW/glew.h>

namespace Nova
{
	class ShaderProgram;

	class ShaderBinder
	{

		static ShaderProgram* m_boundShaderProgram;
		static GLuint         m_bindCount;

	public:
		 ShaderBinder() = delete;
		~ShaderBinder()
		{
		}

		static void BindShaderProgram(ShaderProgram* program);
		static void UnbindShaderProgram();
		static ShaderProgram* GetCurretBoundProgram();
		static GLuint GetAndResetBindCount();


	};

	/*
		m_boundShaderProgram will be set to a default shader but to unbind a shader we cannot set it to 0
		so we need a variable to say weather it is bound or not
	*/


}
