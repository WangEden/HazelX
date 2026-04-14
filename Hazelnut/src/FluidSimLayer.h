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
		void ResetParticles();
		void ResetParameter();
	private:
		//Camera m_Camera;
		OrthographicCamera m_Camera;
		Ref<Framebuffer> m_Framebuffer; // 离屏渲染目标
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;

		// --- 流体物理数据 ---
		std::vector<Particle2D> m_Particles;

		const int PARTICLE_COUNT = 1000;

		int m_ParticleCount = PARTICLE_COUNT;
		float m_Gravity = -9.81f;
		float m_ParticleRadius = 0.08f;
		float m_CollisionDamping = 0.5f;

		float m_BoxWidth = 20.0f;
		float m_BoxHeight = 15.0f;

		float m_SmoothingRadius = 0.25f;
		float m_Poly6ScalingFactor = 4.0f / (glm::pi<float>() * std::pow(m_SmoothingRadius, 8.0f));

		float m_TargetDensity = 1.0f; // 目标密度
		float m_PressureMultiplier = 200.0f; // 压力系数
		float m_ViscosityStrength = 0.05f;    // 粘性系数

		void FluidSimLayer::UpdateScalingFactors()
		{
			// Poly6 核函数用于密度计算的系数 (2D 标准化系数为 4 / (pi * h^8))
			m_Poly6ScalingFactor = 4.0f / (glm::pi<float>() * std::pow(m_SmoothingRadius, 8.0f));

			// 如果你用了 Spiky Kernel，也要在这里更新它的系数
		}

		struct Obstacle {
			glm::vec2 Center = { 0.0f, -2.0f };
			float Radius = 1.0f;
			bool Enabled = true;
		};
		Obstacle m_Obstacle;
	};

}
