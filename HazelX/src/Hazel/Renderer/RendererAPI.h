#pragma once

#include <glm/glm.hpp>

//#include "VertexArray.h"

namespace Hazel {

	using RendererID = uint32_t;

	enum class RendererAPIType
	{
		None = 0,
		OpenGL,
	};

	///////////////////////////////////////////////////////////////////
	////////////////////// Copy from HazelSource //////////////////////
	struct RenderAPICapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;

		int MaxSamples;
		float MaxAnisotropy;
	};
	////////////////////// Copy from HazelSource //////////////////////
	///////////////////////////////////////////////////////////////////

	class RendererAPI
	{
	public:
		///////////////////////////////////////////////////////////////////
		////////////////////// Copy from HazelSource //////////////////////
		static void Init();
		static void Shutdown();

		static void Clear(float r, float g, float b, float a);
		static void SetClearColor(float r, float g, float b, float a);

		static void DrawIndexed(unsigned int count, bool depthTest = true);

		static RenderAPICapabilities& GetCapabilities()
		{
			static RenderAPICapabilities capabilities;
			return capabilities;
		}
		////////////////////// Copy from HazelSource //////////////////////
		///////////////////////////////////////////////////////////////////

		inline static RendererAPIType Current() { return s_CurrentRendererAPI; }
	private:
		static RendererAPIType s_CurrentRendererAPI;
	};
}
