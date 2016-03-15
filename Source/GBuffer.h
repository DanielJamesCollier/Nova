#pragma once
#include <GLEW\glew.h>
#include "GLTexture.h"
namespace Nova
{
	class GBuffer
	{
		enum GBTextures
		{
			GB_POSITION,
			GB_ALBEDOSPEC,
			GB_NORMAL,
			GB_DEPTH,
			GB_BUFFER_SIZE
		};

		GLuint    m_fbo;
		GLTexture m_gBufferTextures[GB_BUFFER_SIZE];
		GLTexture m_finalTexture;
		GLuint    m_width, m_height;
		bool      m_dispised;

		// public functions
	public:
		GBuffer();
		~GBuffer();

		void Init(unsigned int width, unsigned int height);
		void Dispose();

		void BindForReading();
		void BindForGeomPass();
		
		void StartFrame();
		void BindForStencilPass();
		void BindForLightPass();
		void BindForFinalPass();

		void SetReadBuffer(GBTextures type);

		GLuint GetID();

		// private functions
	private:
		void BindTextures();
	};
}