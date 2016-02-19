#pragma once
#include <GLEW\glew.h>

namespace Nova
{
	class GBuffer
	{
	public:

		enum GBTextures
		{
			GB_POSITION,
			GB_ALBEDOSPEC,
			GB_NORMAL,
			GB_BUFFER_SIZE
		};
		
		GBuffer();
		~GBuffer();

		void Init(unsigned int width, unsigned int height);
		void Dispose();

		void BindForReading();
		void BindForGeomPass();
		void BindTextures();

		// new
		void StartFrame();
		void BindForStencilPass();
		void BindForLightPass();
		void BindForFinalPass();

		void SetReadBuffer(GBTextures type);

		GLuint GetID();

	private:

		GLuint m_gBufferTextures[GB_BUFFER_SIZE];
		GLuint m_fbo;
		GLuint m_depth;
		GLuint m_finalTexture;
		GLuint m_width, m_height;
	};
}