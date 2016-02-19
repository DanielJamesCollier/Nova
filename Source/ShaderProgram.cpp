#include "ShaderProgram.h"
#include "Logger.h"
#include "ShaderBinder.h"
#include "ShaderOBJ.h"

#include <iostream>

namespace Nova
{

	ShaderProgram::ShaderProgram(std::string name)
		:
		m_linked(false),
		m_programID(0),
		m_name(name)
	{
		this->m_programID = glCreateProgram();
	}

	ShaderProgram::~ShaderProgram()
	{
		/* detaches the shader objects from the shader program and deletes the program
		- this should not delete the shader objects, this is done in the shader object cache
		*/


		// shaders are detached after linking - so if linking isnt done then they need to be detached here
		if (!m_linked)
			DetachShaderOBJs();
		
		m_shaderOBJs.clear();

		glDeleteProgram(m_programID);
	}

	void ShaderProgram::AddShaderObject(ShaderOBJ* obj)
	{
		if (obj == nullptr)
		{
			Logger::ErrorBlock("Shader Program Error", "Program: " + m_name + "\nError: The shader object that is being added to this program equals null", true);
			return;
		}

		if (m_linked)
		{
			Logger::ErrorBlock("Shader Program Error", "Program: " + m_name + "\nObject : " + obj->GetName() + "\nError: It is not possible to add another shader object to this shader\n it is allready linked", true);
			return;
		}

		if (obj->IsShaderCompiled())
		{
			glAttachShader(m_programID, obj->GetID());
			m_shaderOBJs.push_back(obj);
			return;
		}
		else
		{
			Logger::ErrorBlock("Shader Compile Error", "Program: " + m_name + "\nObject : " + obj->GetName() + "\nError: The shader object being added to the program is not compiled", true);
		}
		
	}

	void ShaderProgram::AddAttribute(GLuint attribute_location, std::string attribute_name)
	{
		glBindAttribLocation(m_programID, attribute_location, attribute_name.c_str()); // TODO
	}

	const std::string& ShaderProgram::GetName() const
	{
		return this->m_name;
	}

	GLint ShaderProgram::GetUniformLocation(std::string uniform_name)
	{
		auto search = m_uniformCache.find(uniform_name);

		if (search == m_uniformCache.end())
		{
			GLint location = glGetUniformLocation(m_programID, uniform_name.c_str());

			if (location != -1)
			{
				m_uniformCache.emplace(uniform_name, location);
				return location;
			}

			Logger::ErrorBlock("GLSL Uniform not found", "Uniform Name: " + uniform_name + "\nLocation: " + std::to_string(location) + "\nProgram: "+ m_name + "\nError: uniform not found in the shader or uniform cache", true);
			return location; // returns -1
		}

		// if found in the map return location
		return search->second;
	}

	GLint ShaderProgram::GetAttributeLocation(std::string attribute_name)
	{
		auto search = m_attributeCache.find(attribute_name);

		if (search == m_attributeCache.end())
		{
			GLint location = glGetAttribLocation(m_programID, attribute_name.c_str());

			if (location != -1)
			{
				std::cout << "attribute \"" << attribute_name << "\"was found in the shader - location: " << location << std::endl;
				m_attributeCache.emplace(attribute_name, location);
				return location;
			}

			Logger::ErrorBlock("GLSL Attribute not found", "Attribute Name: " + attribute_name + "\nLocation: " + std::to_string(location) + "Error: attribute not found in the shader or attribute cache", false);
			return location; // returns -1
		}

		// if found in the map return location
		return search->second;
	}

	int ShaderProgram::LinkProgram()
	{
		

		if (m_linked) return PROGRAM_ALREADY_LINKED;
		if (m_shaderOBJs.empty()) return PROGRAM_LINK_FAILED;

		glLinkProgram(m_programID);

		if (!IsShaderOkay(m_programID, GL_LINK_STATUS, true, "Program: " + this->GetName() + " failed to link"))
		{
			m_linked = false;
			DetachShaderOBJs();
			return PROGRAM_LINK_FAILED;
		}
		
		DetachShaderOBJs();
		m_linked = true;
		return PROGRAM_LINK_SUCCESS;
	}

	ShaderProgram* ShaderProgram::Bind()
	{
		ShaderBinder::BindShaderProgram(this);
		return this;
	}

	void ShaderProgram::Unbind()
	{
		ShaderBinder::UnbindShaderProgram();
	}

	void ShaderProgram::EnableAttribArrays()
	{
		for (unsigned int i = 0; i < m_attributeCache.size(); ++i)	glEnableVertexAttribArray(i);
	}

	void ShaderProgram::DisableAttribArrays()
	{
		for (unsigned int i = 0; i < m_attributeCache.size(); ++i) glDisableVertexAttribArray(i);
	}

	void ShaderProgram::SetUniform1f(GLuint loc, float x)
	{
		glUniform1f(loc, x);
	}

	void ShaderProgram::SetUniform1i(GLuint loc, int x)
	{
		glUniform1i(loc, x);
	}

	void ShaderProgram::SetUniform1i(const std::string& uniform, int x)
	{
		glUniform1i(GetUniformLocation(uniform), x);
	}

	void ShaderProgram::SetUniform1f(const std::string& uniform, float x)
	{
		glUniform1f(GetUniformLocation(uniform), x);
	}

	void ShaderProgram::SetUniform2f(GLuint loc, float x, float y)
	{
		glUniform2f(loc, x, y);
	}

	void ShaderProgram::SetUniform2f(const std::string& uniform, float x, float y)
	{
		glUniform2f(GetUniformLocation(uniform), x, y);
	}

	void ShaderProgram::SetUniform2f(GLuint loc, const glm::vec2& xy)
	{
		glUniform2f(loc, xy.x, xy.y);
	}

	void ShaderProgram::SetUniform2f(const std::string& uniform, const glm::vec2& xy)
	{
		glUniform2f(GetUniformLocation(uniform), xy.x, xy.y);
	}

	void ShaderProgram::SetUniform3f(GLuint loc, float x, float y, float z)
	{
		glUniform3f(loc, x, y, z);
	}

	void ShaderProgram::SetUniform3f(const std::string& uniform, float x, float y, float z)
	{
		glUniform3f(GetUniformLocation(uniform), x, y, z);
	}

	void ShaderProgram::SetUniform3f(GLuint loc, const glm::vec3& xyz)
	{
		glUniform3f(loc , xyz.x, xyz.y, xyz.z);
	}

	void ShaderProgram::SetUniform3f(const std::string& uniform, const glm::vec3& xyz)
	{
		glUniform3f(GetUniformLocation(uniform), xyz.x, xyz.y, xyz.z);
	}

	void ShaderProgram::SetUniform4f(GLuint loc, float x, float y, float z, float w)
	{
		glUniform4f(loc, x, y, z, w);
	}

	void ShaderProgram::SetUniform4f(const std::string& uniform, float x, float y, float z, float w)
	{
		glUniform4f(GetUniformLocation(uniform), x, y, z, w);
	}

	void ShaderProgram::SetUniform4f(GLuint loc, const glm::vec4& xyzw)
	{
		glUniform4f(loc, xyzw.x, xyzw.y, xyzw.z, xyzw.w);
	}

	void ShaderProgram::SetUniform4f(const std::string& uniform, const glm::vec4& xyzw)
	{
		glUniform4f(GetUniformLocation(uniform), xyzw.x, xyzw.y, xyzw.z, xyzw.w);
	}

	void ShaderProgram::SetUniform4m(GLuint loc, const float m[16])
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, m);
	}

	void ShaderProgram::SetUniform4m(const std::string& name, const float m[16])
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, m);
	}

	void ShaderProgram::SetUniform4m(GLuint loc, const glm::mat4& m)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}

	void ShaderProgram::SetUniform4m(const std::string& name, const glm::mat4& m)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}

	bool ShaderProgram::IsLinked()
	{
		return this->m_linked;
	}

	GLint ShaderProgram::GetID()
	{
		return m_programID;
	}

	bool ShaderProgram::IsShaderOkay(int shader, int flag, bool isProgram, const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		if (isProgram)
			glGetProgramiv(shader, flag, &success);
		else
			glGetShaderiv(shader, flag, &success);

		if (!success)
		{
			if (isProgram)
				glGetProgramInfoLog(shader, sizeof(error), NULL, error);
			else
				glGetShaderInfoLog(shader, sizeof(error), NULL, error);

			Logger::ErrorBlock(errorMessage, error, true);
			return false;
		}
		return true;
	}

	void ShaderProgram::DetachShaderOBJs()
	{
		for (auto b : m_shaderOBJs)
		{
			glDetachShader(m_programID, b->GetID());
		}
	}
}