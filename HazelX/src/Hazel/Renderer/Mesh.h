#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	///////////////////////////////////////////////////////////////////
	////////////////////// Copy from HazelSource //////////////////////
	class Mesh
	{
	public:
		struct Vertex // 内存布局需要和着色器中的layout一致
		{
			glm::vec3 Position; // 位置
			glm::vec3 Normal;	// 法线
			glm::vec3 Tangent;	// 切线
			glm::vec3 Binormal;	// 副切线
			glm::vec2 Texcoord;	// 纹理坐标（U, V）
		};
		static_assert(sizeof(Vertex) == 14 * sizeof(float));
		static const int NumAttributes = 5;

		struct Index
		{
			uint32_t V1, V2, V3;
		};
		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

		Mesh(const std::string& filename);
		~Mesh();

		void Render();

		inline const std::string& GetFilePath() const { return m_FilePath; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		std::string m_FilePath;
	};

	////////////////////// Copy from HazelSource //////////////////////
	///////////////////////////////////////////////////////////////////
}
