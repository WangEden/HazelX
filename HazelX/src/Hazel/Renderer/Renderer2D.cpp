#include "hzpch.h"

#include "Renderer2D.h"
#include "Hazel/Core/Base.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct CircleVertex {
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade; // 边缘抗锯齿淡出
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;

		// --- 矩形批处理数据 ---
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// --- 圆形批处理数据 ---
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		// ==================== 1. 初始化 Quad 批处理 ====================
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// ==================== 2. 初始化 Circle 批处理 ====================
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float,  "a_Thickness" },
			{ ShaderDataType::Float,  "a_Fade" }
			});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);

		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// ==================== 3. 共享的 Index Buffer ====================
		// 预先计算好所有矩形的索引 (0,1,2, 2,3,0 ...)
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices * sizeof(uint32_t));
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // 圆形本质上也是画矩形，然后用 Shader 裁剪
		delete[] quadIndices;

		// ==================== 4. 加载 Shader ====================
		// 请确保你在 assets/shaders 下有这两个文件
		s_Data.QuadShader = Shader::Create("assets/shaders/Quad-FluidTest.glsl");
		s_Data.CircleShader = Shader::Create("assets/shaders/Circle-FluidTest.glsl");
	}

	void Renderer2D::Shutdown() {
		delete[] s_Data.QuadVertexBufferBase;
		delete[] s_Data.CircleVertexBufferBase;
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjection) {
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
	}

	void Renderer2D::EndScene() {
		Flush();
	}

	void Renderer2D::Flush() {
		// 1. 渲染矩形批次
		if (s_Data.QuadIndexCount > 0) {
			// 上传数据到 GPU
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			// 绑定 Shader
			s_Data.QuadShader->Bind();

			// 绑定对应的 VAO (关键修改)
			s_Data.QuadVertexArray->Bind();

			// 提交绘制指令，2D 渲染通常关闭深度测试以依赖绘制顺序，所以传入 false
			Renderer::DrawIndexed(s_Data.QuadIndexCount, false);
		}

		// 2. 渲染圆形批次
		if (s_Data.CircleIndexCount > 0) {
			// 上传数据到 GPU
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			// 绑定 Shader
			s_Data.CircleShader->Bind();

			// 绑定对应的 VAO (关键修改)
			s_Data.CircleVertexArray->Bind();

			// 提交绘制指令
			Renderer::DrawIndexed(s_Data.CircleIndexCount, false);
		}
	}

	void Renderer2D::FlushAndReset() {
		Flush();
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::FlushAndResetCircles() {
		Flush();
		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f))
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		const glm::vec4 quadVertexPositions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		const glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		for (size_t i = 0; i < 4; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * quadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color, float thickness) {
		if (s_Data.CircleIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndResetCircles();
		}

		// 将半径放大两倍作为矩形的宽高，因为 LocalPosition 从 -1 到 1
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(radius * 2.0f));

		const glm::vec4 quadVertexPositions[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};

		for (size_t i = 0; i < 4; i++) {
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * quadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = quadVertexPositions[i] * 2.0f; // 映射到 [-1, 1] 范围
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = 0.005f; // 可以作为 uniform 或参数暴露
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;
	}
}
