#include "hzpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Ripple {

    Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None:     return nullptr;
            case RendererAPIType::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(data, size, usage);
        }
        RP_CORE_ASSERT(false, "Unknown RendererAPI");

        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
    {
        switch (RendererAPI::Current())
        {
        case RendererAPIType::None:    return nullptr;
        case RendererAPIType::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(size, usage);
        }
        RP_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None:     return nullptr;
            case RendererAPIType::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(size);
        }
        RP_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
    {
        switch (RendererAPI::Current())
        {
        case RendererAPIType::None:    return nullptr;
        case RendererAPIType::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(data, size);
        }
        RP_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
