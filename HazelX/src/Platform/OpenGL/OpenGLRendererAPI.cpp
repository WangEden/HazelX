#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Hazel {

	///////////////////////////////////////////////////////////////////
	////////////////////// Copy from HazelSource //////////////////////
	void RendererAPI::Init()
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void RendererAPI::Shutdown()
	{
	}

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void RendererAPI::DrawIndexed(unsigned int count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	////////////////////// Copy from HazelSource //////////////////////
	///////////////////////////////////////////////////////////////////

	//void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	//{
	//	glClearColor(color.r, color.g, color.b, color.a);
	//}

	//void OpenGLRendererAPI::Clear()
	//{
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}

	//void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	//{
	//	vertexArray->Bind();
	//	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);
	//}

}
