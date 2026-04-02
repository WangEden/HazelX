#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {
	// 帧缓冲区对象 FBO
	// 之前没有直接指定帧缓冲区的时候，默认会渲染到一个默认的帧缓冲区上，
	// 这个默认的帧缓冲区由窗口系统创建，通常与窗口的大小相同。
	// 这个默认的帧缓冲区和显示器直接相关，渲染到这个帧缓冲区上的内容会直接显示在屏幕上。
	// 当我们需要进行离屏渲染或者后期处理时，我们就需要创建一个自定义的帧缓冲区对象 FBO。

	OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format)
		: m_Width(width), m_Height(height), m_Format(format)
	{
		Resize(width, height);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		HZ_RENDER_S({
			glDeleteFramebuffers(1, &self->m_RendererID);
		});
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (m_Width == width && m_Height == height)
			return;

		m_Width = width;
		m_Height = height;
		HZ_RENDER_S({
			if (self->m_RendererID)
			{
				glDeleteFramebuffers(1, &self->m_RendererID);
				glDeleteTextures(1, &self->m_ColorAttachment);
				glDeleteTextures(1, &self->m_DepthAttachment);
			}

			glGenFramebuffers(1, &self->m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererID);

			glGenTextures(1, &self->m_ColorAttachment);
			glBindTexture(GL_TEXTURE_2D, self->m_ColorAttachment);

			// TODO: Create Hazel texture object based on format here
			if (self->m_Format == FramebufferFormat::RGBA16F)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, self->m_Width, self->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
			}
			else if (self->m_Format == FramebufferFormat::RGBA8)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->m_Width, self->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->m_ColorAttachment, 0);

			glGenTextures(1, &self->m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, self->m_DepthAttachment);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, self->m_Width, self->m_Height, 0,
				GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
			);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->m_DepthAttachment, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				HZ_CORE_ERROR("Framebuffer is incomplete!");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFramebuffer::Bind() const
	{
		HZ_RENDER_S({
			glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererID);
			glViewport(0, 0, self->m_Width, self->m_Height);
		});
	}

	void OpenGLFramebuffer::Unbind() const
	{
		HZ_RENDER_S({
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFramebuffer::BindTexture(uint32_t slot) const
	{
		HZ_RENDER_S1(slot, {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, self->m_ColorAttachment);
		});
	}
}
