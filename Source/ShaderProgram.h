#pragma once
#ifndef SHADER_PROGRAM_HEADER
#define SHADER_PROGRAM_HEADER

#include <vector>
#include <string>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <unordered_map>

// addAttribute(); function infomation

/* method used if you want to explicetly say what location you want to give to the atribute
THIS METHOD MUST BE USED BEFORE LINKING, otherwise the attributes will assigned locations
by the glsl compiler, to get this locations use the getAttributeLocation() function provided in
this class
*/




namespace Nova
{

	class ShaderOBJ;

	class ShaderProgram
	{
	public:


		/*Constructors & Destructor*/
		/*--------------------------*/
		explicit ShaderProgram(std::string name);
		~ShaderProgram();
		/*--------------------------*/


		/*Adders*/
		/*--------------------------*/
		void AddShaderObject(ShaderOBJ* obj);
		void AddAttribute(GLuint attribute_location, std::string attribute_name);
		/*--------------------------*/


		/*Getters*/
		/*--------------------------*/
		const std::string& GetName() const;
		GLint GetUniformLocation(std::string uniform_name);    // TODO change to private ?
		GLint GetAttributeLocation(std::string attribute_name); // TODO change to private ?
		GLint GetID();
		bool  IsLinked();
		GLint getNumberOfUniforms(); // TODO
		GLint getNumberOfAttributes(); // TODO
		/*--------------------------*/


		/*Setters*/
		/*--------------------------*/
		void SetUniform1i(GLuint loc, int x);
		void SetUniform1i(const std::string& uniform, int x);

		void SetUniform1f(GLuint loc, float x);
		void SetUniform1f(const std::string& uniform, float x);

		void SetUniform2f(GLuint loc, float x, float y);
		void SetUniform2f(const std::string& uniform, float x, float y);
		void SetUniform2f(GLuint loc, const glm::vec2& xy);
		void SetUniform2f(const std::string& uniform, const glm::vec2& xy);

		void SetUniform3f(GLuint loc, float x, float y, float z);
		void SetUniform3f(const std::string& uniform, float x, float y, float z);
		void SetUniform3f(GLuint loc, const glm::vec3& xyz);
		void SetUniform3f(const std::string& uniform, const glm::vec3& xyz);

		void SetUniform4f(GLuint loc, float x, float y, float z, float w);
		void SetUniform4f(const std::string& uniform, float x, float y, float z, float w);
		void SetUniform4f(GLuint loc, const glm::vec4& xyzw);
		void SetUniform4f(const std::string& uniform, const glm::vec4& xyzw);

		void SetUniform4m(GLuint loc, const float m[16]);
		void SetUniform4m(const std::string& name, const float m[16]);
		void SetUniform4m(GLuint loc, const glm::mat4& m);
		void SetUniform4m(const std::string& name, const glm::mat4& m);
		/*--------------------------*/


		/*Shader Control*/
		/*--------------------------*/
		int LinkProgram();
		void Unbind();
		ShaderProgram* Bind();
		void EnableAttribArrays();
		void DisableAttribArrays();
		/*--------------------------*/

		bool IsShaderOkay(int shader, int flag, bool isProgram, const std::string& errorMessage);

		enum
		{
			PROGRAM_ALREADY_LINKED = -2,
			PROGRAM_LINK_SUCCESS   =  1,
			PROGRAM_LINK_FAILED    = -1
		};

	private:
		bool		m_linked;
		std::string m_name;
		GLuint		m_programID;

		std::vector<ShaderOBJ*>													m_shaderOBJs;
		std::unordered_map<std::string /*uniform_name*/, GLuint /*location*/ >  m_uniformCache;
		std::unordered_map<std::string /*attribute name*/, GLuint /*location*/> m_attributeCache;

		void DetachShaderOBJs();
	};
}
#endif //SHADER_PROGRAM_HEADER