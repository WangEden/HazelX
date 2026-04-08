#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Renderer.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& viewProjection);
		static void EndScene();
		static void Flush();

		// SPH 仿真最需要的：画四边形和画圆
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color, float thickness = 1.0f);

	private:
		static void FlushAndReset();
		static void FlushAndResetCircles();
	};
}
