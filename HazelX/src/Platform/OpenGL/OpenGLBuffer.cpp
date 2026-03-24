#include "hzpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {

	// ==================================== Vertex Buffer ============================

	///////////////////////////////////////////////////////////////////
	////////////////////// Copy from HazelSource //////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		HZ_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		HZ_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
		});
	}

	void OpenGLVertexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
	{
		HZ_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		});

	}

	void OpenGLVertexBuffer::Bind() const
	{
		HZ_RENDER_S({
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
		});
	}
	////////////////////// Copy from HazelSource //////////////////////
	///////////////////////////////////////////////////////////////////

	//OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	//{
	//	glCreateBuffers(1, &m_RendererID);
	//	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // 将顶点数据传输到GPU,
	//}

	//OpenGLVertexBuffer::~OpenGLVertexBuffer()
	//{
	//	glDeleteBuffers(1, &m_RendererID);
	//}

	//void OpenGLVertexBuffer::Bind() const
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	//}

	//void OpenGLVertexBuffer::Unbind() const
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}

	// ==================================== Index Buffer ============================

	///////////////////////////////////////////////////////////////////
	////////////////////// Copy from HazelSource //////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		HZ_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
		});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZ_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
		});
	}

	void OpenGLIndexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
	{
		HZ_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
		});
	}

	void OpenGLIndexBuffer::Bind() const
	{
		HZ_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
		});
	}
	////////////////////// Copy from HazelSource //////////////////////
	///////////////////////////////////////////////////////////////////

	//OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
	//	: m_Count(count)
	//{
	//	glCreateBuffers(1, &m_RendererID);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW); // 将顶点数据传输到GPU,
	//}

	//OpenGLIndexBuffer::~OpenGLIndexBuffer()
	//{
	//	glDeleteBuffers(1, &m_RendererID);
	//}

	//void OpenGLIndexBuffer::Bind() const
	//{
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	//}

	//void OpenGLIndexBuffer::Unbind() const
	//{
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//}

}
