#pragma once
#include <string>

namespace Nova
{
	class ShaderProgram;

	class ShaderBinder
	{
	public:
		 ShaderBinder() = delete;
		~ShaderBinder()
		{
		}

		static void BindShaderProgram(ShaderProgram* program);
		static void UnbindShaderProgram();
		static ShaderProgram* GetCurretBoundProgram();

	private:
		static ShaderProgram* m_boundShaderProgram;
	};

	/*
		m_boundShaderProgram will be set to a default shader but to unbind a shader we cannot set it to 0
		so we need a variable to say weather it is bound or not
	*/


}
