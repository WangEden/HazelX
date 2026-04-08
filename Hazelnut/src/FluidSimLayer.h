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

		float CalculateDensity(glm::vec2 samplePoint)
		{
			float density = 0.0f;
			const float mass = 1.0f; // 假设每个粒子的质量为 1

			for (const glm::vec2& position : m_Positions)
			{
				// 两点间的直线距离
				float dst = glm::distance(position, samplePoint);
				float influence = Physics2D::SmoothingKernel(m_SmoothingRadius, dst, m_Poly6ScalingFactor);
				density += mass * influence;
			}

			return density;
		}

	private:
		//Camera m_Camera;
		OrthographicCamera m_Camera;
		Ref<Framebuffer> m_Framebuffer; // 离屏渲染目标
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;

		// --- 流体物理数据 ---
		std::vector<glm::vec2> m_Positions;
		std::vector<glm::vec2> m_Velocities;

		int m_ParticleCount = 1000;
		float m_Gravity = -9.8f;
		float m_ParticleRadius = 0.16f;
		float m_CollisionDamping = 0.7f;

		float m_BoxWidth = 20.0f;
		float m_BoxHeight = 15.0f;

		float m_SmoothingRadius = 0.48f;
		float m_Poly6ScalingFactor = 4.0f / (glm::pi<float>() * std::pow(m_SmoothingRadius, 8.0f));
	};

}
