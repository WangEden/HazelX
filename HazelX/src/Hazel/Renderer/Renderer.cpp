#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	///////////////////////////////////////////////////////////////////
	////////////////////// Copy from HazelSource //////////////////////
	Renderer* Renderer::s_Instance = new Renderer();
	RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

	void Renderer::Init()
	{
		HZ_RENDER({ RendererAPI::Init(); });
	}

	void Renderer::Clear()
	{
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		HZ_RENDER_4(r, g, b, a, {
			RendererAPI::Clear(r, g, b, a);
			});
	}

	void Renderer::ClearMagenta()
	{
		Clear(1, 0, 1);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
	}

	void Renderer::DrawIndexed(unsigned int count)
	{
		HZ_RENDER_1(count, {
			RendererAPI::DrawIndexed(count);
			});
	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}
	////////////////////// Copy from HazelSource //////////////////////
	///////////////////////////////////////////////////////////////////

	//void Renderer::BeginScene(OrthographicCamera& camera)
	//{
	//	m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	//}

	//void Renderer::EndScene()
	//{
	//}

	//void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray)
	//{
	//	shader->Bind();
	//	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

	//	vertexArray->Bind();
	//	RenderCommand::DrawIndexed(vertexArray);
	//}
	 
}
