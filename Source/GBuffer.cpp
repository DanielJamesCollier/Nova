#include "GBuffer.h"
#include "Logger.h"
#include "TextureBinder.h"

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

		// generate gBufferTextures
		for (unsigned int i = 0; i < GBTextures::GB_BUFFER_SIZE; ++i)
		{
			glGenTextures(1, &m_gBufferTextures[i].id);
			m_gBufferTextures[i].type = GL_TEXTURE_2D;
			m_gBufferTextures[i].width = width;
			m_gBufferTextures[i].height = height;
		}
		// generate final texture
		glGenTextures(1, &m_finalTexture.id);
		m_finalTexture.width = width;
		m_finalTexture.height = height;
		m_finalTexture.type = GL_TEXTURE_2D;

		//Texture binder refrence
		TextureBinder& texturebind = TextureBinder::GetInstance();

		// position attachment
		texturebind.BindTexture(0, &m_gBufferTextures[GBTextures::GB_POSITION]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gBufferTextures[0].id, 0);

		// albedo & specular intensity rgb = texture colour a = specular intensity
		texturebind.BindTexture(0, &m_gBufferTextures[GBTextures::GB_ALBEDOSPEC]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gBufferTextures[1].id, 0);

		// normal attachment RGB = normal | a is free
		texturebind.BindTexture(0, &m_gBufferTextures[GBTextures::GB_NORMAL]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gBufferTextures[2].id, 0);

		// depth
		texturebind.BindTexture(0, &m_gBufferTextures[GBTextures::GB_DEPTH]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_gBufferTextures[3].id, 0);

		// final
		texturebind.BindTexture(0, &m_finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_finalTexture.id, 0);

		GLenum drawBuffers[] =
		{
			GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2
		};

		glDrawBuffers(3, drawBuffers);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status == GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::GetInstance().InfoBlock("Frame Buffer Created", "the GBuffer was created for the new deferred renderer", true);
		}

		// restore default fbo
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::Dispose()
	{
		glBindBuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &m_fbo);

		for (unsigned int i = 0; i < GBTextures::GB_BUFFER_SIZE; ++i)
		{
			glDeleteTextures(1, &m_gBufferTextures[i].id);
		}
	}

	void GBuffer::BindForReading()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		BindTextures();
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
		TextureBinder& texturebind = TextureBinder::GetInstance();
		texturebind.BindTexture(0, &m_gBufferTextures[GBTextures::GB_POSITION]);
		texturebind.BindTexture(1, &m_gBufferTextures[GBTextures::GB_ALBEDOSPEC]);
		texturebind.BindTexture(2, &m_gBufferTextures[GBTextures::GB_NORMAL]);
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

