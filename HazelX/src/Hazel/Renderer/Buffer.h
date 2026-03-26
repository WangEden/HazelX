#pragma once

#include "Hazel/Renderer/Renderer.h"

namespace Hazel {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		///////////////////////////////////////////////////////////////////
		////////////////////// Copy from HazelSource //////////////////////
		virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void Bind() const = 0;

		virtual unsigned int GetSize() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static VertexBuffer* Create(unsigned int size = 0);
		////////////////////// Copy from HazelSource //////////////////////
		///////////////////////////////////////////////////////////////////
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		///////////////////////////////////////////////////////////////////
		////////////////////// Copy from HazelSource //////////////////////
		virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void Bind() const = 0;

		virtual uint32_t GetCount() const = 0;

		virtual unsigned int GetSize() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static IndexBuffer* Create(unsigned int size = 0);
		////////////////////// Copy from HazelSource //////////////////////
		///////////////////////////////////////////////////////////////////
	};
}
