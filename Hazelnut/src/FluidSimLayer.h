#pragma once

#include "Hazel.h"

#include "Hazel/ImGui/ImGuiLayer.h"
#include "imgui/imgui_internal.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <algorithm>

namespace Hazel {

	class FluidSimLayer : public Layer
	{
	public:
		FluidSimLayer();
		virtual ~FluidSimLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		void ResetAll();
		void ResetParticles();

		void ResetParameter() {
			m_ParticleCount = 2500;
			m_Gravity = -9.81f;
			//m_Gravity = 0.0f;

			m_ParticleRadius = 0.04f;
			m_CollisionDamping = 1.0f;		// 碰撞阻尼，1.0表示弹性碰撞

			m_SmoothingRadius = 0.25f;
			m_TargetDensity = 8.0f;			// 目标密度
			m_PressureMultiplier = 20.0f;	// 压力系数，值越大越难压缩
			m_ViscosityStrength = 1.5f;		// 粘性系数，液体的粘稠度，越高液体越粘稠

			m_BoxWidth = 15.0;
			m_BoxHeight = 10.0f;

			m_Obstacle.Center = { -2.0f, -3.0f };
			m_Obstacle.Radius = 1.0f;
			m_Obstacle.Enabled = true;
		}

		uint32_t CalculateHash(int gx, int gy) {
			return ((uint32_t)gx * 73856093 ^ (uint32_t)gy * 19349663) % m_TableSize;
		}

		void BuildSpatialHash() {
			m_SortedEntries.resize(m_Particles.size());
			m_CellStart.assign(m_TableSize + 1, 0xFFFFFFFF); // 初始化为无效索引

			float invH = 1.0f / m_SmoothingRadius;

			for (int i = 0; i < m_Particles.size(); i++) {
				int gx = (int)std::floor(m_Particles[i].Position.x * invH);
				int gy = (int)std::floor(m_Particles[i].Position.y * invH);
				m_SortedEntries[i] = { CalculateHash(gx, gy), (uint32_t)i };
			}

			std::sort(m_SortedEntries.begin(), m_SortedEntries.end(), [](const auto& a, const auto& b) {
				return a.Hash < b.Hash;
				});

			for (uint32_t i = 0; i < m_SortedEntries.size(); i++) {
				uint32_t hash = m_SortedEntries[i].Hash;
				uint32_t prevHash = (i == 0) ? 0xFFFFFFFF : m_SortedEntries[i - 1].Hash;
				if (hash != prevHash) {
					m_CellStart[hash] = i;
				}
			}
		}
		bool OnKeyPressedEvent(KeyPressedEvent& e);

	private:
		OrthographicCamera m_Camera;
		Ref<Framebuffer> m_Framebuffer; // 离屏渲染目标
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		std::vector<Particle2D> m_Particles;

		int m_ParticleCount = 0;
		float m_Gravity = 0.0f;
		float m_ParticleRadius = 0.0f;
		float m_CollisionDamping = 0.0f;

		float m_BoxWidth = 0.0f;
		float m_BoxHeight = 0.0f;

		float m_SmoothingRadius = 0.0f;

		float m_TargetDensity = 0.0f;
		float m_PressureMultiplier = 0.0f;
		float m_ViscosityStrength = 0.0f;

		int m_HashGridStep = 1; // Hash邻域半径

		struct Obstacle {
			glm::vec2 Center = { 0.0f, -2.0f };
			float Radius = 1.0f;
			bool Enabled = true;
		};
		Obstacle m_Obstacle;

		struct ParticleEntry { // 空间网格hash
			uint32_t Hash;
			uint32_t Index;
		};

		std::vector<ParticleEntry> m_SortedEntries; // 存储排序后的条目
		std::vector<uint32_t> m_CellStart; // 存储每个哈希值在 SortedEntries 中的起始位置
		const uint32_t m_TableSize = 65536; // 假设哈希表大小为 2^16 = 65536

		bool resume = true;

		float m_InteractionRadius = 4.0f;
		float m_InteractionStrength = 30.0f;
		glm::vec2 m_ViewportBounds[2];
		
	};

}
