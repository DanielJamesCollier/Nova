#include "GBuffer.h"
#include "Logger.h"

namespace Nova
{
	GBuffer::GBuffer()
	{
	}


	GBuffer::~GBuffer()
	{
		Dispose();
	}

	void GBuffer::Init(unsigned int width, unsigned int height)
	{
		this->m_width = width;
		this->m_height = height;

		// fbo
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// create textures
		glGenTextures(GBTextures::GB_BUFFER_SIZE, m_gBufferTextures);
		glGenTextures(1, &m_depth);

		// position attachment
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBufferTextures[0], 0);

		// albedo & specular intensity rgb = texture colour a = specular intensity
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBufferTextures[1], 0);

		// normal attachment RGB = normal | a is free
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBufferTextures[2], 0);

		// depth
		glGenTextures(1, &m_depth);
		glBindTexture(GL_TEXTURE_2D, m_depth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

		// final
		glGenTextures(1, &m_finalTexture);
		glBindTexture(GL_TEXTURE_2D, m_finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_finalTexture, 0);

		GLenum drawBuffers[] =
		{
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2
		};

		glDrawBuffers(3, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status == GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::InfoBlock("Frame Buffer Created", "the GBuffer was created for the new deferred renderer", true);
		}

		// restore default fbo
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::Dispose()
	{
		glBindBuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &m_fbo);
		glDeleteTextures(GBTextures::GB_BUFFER_SIZE, m_gBufferTextures);
		glDeleteTextures(1, &m_depth);
	}

	void GBuffer::BindForReading()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBTextures::GB_POSITION]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBTextures::GB_ALBEDOSPEC]);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBTextures::GB_NORMAL]);
	}

	void GBuffer::StartFrame()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GBuffer::BindForGeomPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

		GLenum DrawBuffers[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2 };

		glDrawBuffers(3, DrawBuffers);
	}

	void GBuffer::BindForStencilPass()
	{
		glDrawBuffer(GL_NONE);
	}

	void GBuffer::BindForLightPass()
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		BindTextures();
	}

	void GBuffer::BindForFinalPass()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		glReadBuffer(GL_COLOR_ATTACHMENT3);
	}

	void GBuffer::BindTextures()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBTextures::GB_POSITION]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBTextures::GB_ALBEDOSPEC]);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBTextures::GB_NORMAL]);
	}

	void GBuffer::SetReadBuffer(GBTextures type)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + type);
	}

	GLuint GBuffer::GetID()
	{
		return m_fbo;
	}

}

