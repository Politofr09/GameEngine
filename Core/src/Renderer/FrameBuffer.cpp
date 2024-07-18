#include "FrameBuffer.h"
#include <Core/Utils.h>
#include <gl/glew.h>

namespace Core::Gfx
{

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	{
		Create(width, height);
	}

	void FrameBuffer::Create(unsigned int width, unsigned int height)
	{
		m_Width = width;
		m_Height = height;

		glGenFramebuffers(1, &m_FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG_ERROR("Frame buffer not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void FrameBuffer::Resize(unsigned int width, unsigned int height)
	{
		if (m_Width == width && m_Height == height) return;

		m_Width = width;
		m_Height = height;

		Free();
		Create(width, height);
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		// We've bound the frame buffer; this means we want to update the rendering area dimensions
		// We can do so by triggering a ViewportResizedEvent event
		Events::ViewportResizedEvent* event = new Events::ViewportResizedEvent(m_Width, m_Height);
		Events::Dispatcher::Trigger(event);
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void FrameBuffer::Free()
	{
		glDeleteTextures(1, &m_TextureID);
		glDeleteRenderbuffers(1, &m_RenderBufferID);
		glDeleteFramebuffers(1, &m_FrameBufferID);
	}

}
