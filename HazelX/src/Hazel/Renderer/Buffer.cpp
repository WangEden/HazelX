#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

    //VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    VertexBuffer* VertexBuffer::Create(unsigned int size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: HZ_CORE_ASSERT(false, "RendererAPI::API::None is not support"); return nullptr;
            //case RendererAPIType::OpenGL: return new OpenGLVertexBuffer(vertices, size);
            case RendererAPIType::OpenGL: return new OpenGLVertexBuffer(size);
        }

        HZ_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    //IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    IndexBuffer* IndexBuffer::Create(unsigned int size)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::None: HZ_CORE_ASSERT(false, "RendererAPI::API::None is not support"); return nullptr;
            //case RendererAPIType::OpenGL: return new OpenGLIndexBuffer(indices, count);
            case RendererAPIType::OpenGL: return new OpenGLIndexBuffer(size);
        }

        HZ_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
