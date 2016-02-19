#pragma once
#include "DeferredPass.h"
#include "GLEW\glew.h"

namespace Nova
{
	class LightPass : public DeferredPass
	{
	public:
		LightPass();
		~LightPass();

		virtual bool Init();

		void SetMVP(const glm::mat4& MVP);
		void SetPositionTextureUnit(unsigned int TextureUnit);
		void SetDiffuseTextureUnit(unsigned int TextureUnit);
		void SetNormalTextureUnit(unsigned int TextureUnit);
		void SetEyeWorldPos(const glm::vec3& EyeWorldPos);
		void SetMatSpecularPower(float Power);
		void SetScreenSize(unsigned int Width, unsigned int Height);

	private:
		// SHADER UNIFORM LOCATION

		// g buffer
		GLuint m_positionTextureUnit;
		GLuint m_diffuseTextureUnit;
		GLuint m_normalTextureUnit;

		// other
		GLuint m_MVP;
		GLuint m_screenSize;
		GLuint m_specularPower;
		GLuint m_eyeWorlPos;
	};


}

