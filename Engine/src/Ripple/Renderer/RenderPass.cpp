#include "hzpch.h"
#include "RenderPass.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLRenderPass.h"

// Copy from RippleSource

namespace Ripple {

	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:    RP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPIType::OpenGL:  return Ref<OpenGLRenderPass>::Create(spec);
		}

		RP_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
