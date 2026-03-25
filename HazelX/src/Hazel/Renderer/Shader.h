#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Renderer/Renderer.h"

#include <string>
#include <glm/glm.hpp>

namespace Hazel {

	struct ShaderUniform
	{

	};

	struct ShaderUniformCollection
	{

	};

	enum class UniformType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Matrix3x3, Matrix4x4,
		Int32, Uint32
	};

	struct UniformDecl
	{
		UniformType Type;
		std::ptrdiff_t Offset;
		std::string Name;
	};

	struct UniformBuffer
	{
		// TODO: This currently represents a byte buffer that has been
		// packed with uniforms. This was primarily created for OpenGL,
		// and needs to be revisted for other rendering APIs. Furthermore,
		// this currently does not assume any alignment. This also has
		// nothing to do with GL uniform buffers, this is simply a CPU-side
		// buffer abstraction.
		byte* Buffer;
		std::vector<UniformDecl> Uniforms;
	};

	struct UniformBufferBase
	{
		virtual const byte* GetBuffer() const = 0;
		virtual const UniformDecl* GetUniforms() const = 0;
		virtual unsigned int GetUniformCount() const = 0;
	};

	template<unsigned int N, unsigned int U>
	struct UniformBufferDeclaration : public UniformBufferBase
	{
		byte Buffer[N];
		UniformDecl Uniforms[U];
		std::ptrdiff_t Cursor = 0;
		unsigned int UniformIndex = 0;

		virtual const byte* GetBuffer() const override { return Buffer; }
		virtual const UniformDecl* GetUniforms() const override { return Uniforms; }
		virtual unsigned int GetUniformCount() const { return U; }

		template<typename T>
		void Push(const std::string& name, const T& data)
		{
			if (UniformIndex >= U) return;

			if constexpr (std::is_same_v<T, float>) {
				Uniforms[UniformIndex] = { UniformType::Float, (uint32_t)Cursor, name };
				memcpy(Buffer + Cursor, &data, sizeof(float));
				Cursor += sizeof(float);
			}
			else if constexpr (std::is_same_v<T, glm::vec4>) {
				Uniforms[UniformIndex] = { UniformType::Float4, (uint32_t)Cursor, name };
				memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
				Cursor += sizeof(glm::vec4);
			}

			UniformIndex++;
		}

	};

	class Shader 
	{
	public:
		virtual void Bind() = 0;
		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;

		static Shader* Create(const std::string& filepath);
	};
  
}
