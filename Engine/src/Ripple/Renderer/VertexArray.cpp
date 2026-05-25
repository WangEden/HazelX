#include "hzpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ripple {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    RP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPIType::OpenGL:  return Ref<OpenGLVertexArray>::Create();
		}

		RP_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}
