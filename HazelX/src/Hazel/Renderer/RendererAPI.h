#pragma once

#include <glm/glm.hpp>

//#include "VertexArray.h"

namespace Hazel {

	using RendererID = unsigned int;

	enum class RendererAPIType
	{
		None = 0, OpenGL = 1,
	};

	class RendererAPI
	{
	public:
		///////////////////////////////////////////////////////////////////
		////////////////////// Copy from HazelSource //////////////////////
		static void Init();
		static void Shutdown();
		static void Clear(float r, float g, float b, float a);
		static void SetClearColor(float r, float g, float b, float a);
		static void DrawIndexed(unsigned int count);
		////////////////////// Copy from HazelSource //////////////////////
		///////////////////////////////////////////////////////////////////

		//virtual void SetClearColor(const glm::vec4& color) = 0;
		//virtual void Clear() = 0;
		//virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static RendererAPIType Current() { return s_CurrentRendererAPI; }
	private:
		//static API s_API;
		static RendererAPIType s_CurrentRendererAPI;
	};
}
