#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        //OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const;
        //virtual void Unbind() const;

        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout& layout) override {
            m_Layout = layout;
        }

        ///////////////////////////////////////////////////////////////////
        ////////////////////// Copy from HazelSource //////////////////////
        OpenGLVertexBuffer(unsigned int size);
        virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0);
        virtual unsigned int GetSize() const { return m_Size; }
        virtual RendererID GetRendererID() const { return m_RendererID; }
        ////////////////////// Copy from HazelSource //////////////////////
        ///////////////////////////////////////////////////////////////////

    private:
        RendererID m_RendererID;
        BufferLayout m_Layout;
        unsigned int m_Size;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        //OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        //virtual void Unbind() const;

        virtual uint32_t GetCount() const { return m_Count; }

        ///////////////////////////////////////////////////////////////////
        //////////////////////// Copy from HazelSource ////////////////////
        OpenGLIndexBuffer(unsigned int size);
        virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0);
        virtual unsigned int GetSize() const { return m_Size; }
        virtual RendererID GetRendererID() const { return m_RendererID; }
        ////////////////////// Copy from HazelSource //////////////////////
        ///////////////////////////////////////////////////////////////////
    private:
        RendererID m_RendererID;
        uint32_t m_Count;
        unsigned int m_Size;
    };

}
