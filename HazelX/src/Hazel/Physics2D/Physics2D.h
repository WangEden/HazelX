#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#include <glm/gtx/type_aligned.hpp>

namespace Hazel {

	struct Particle2D
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		float Density;
		float Property;
	};

	class Physics2D
	{
	public:
		static bool PointInAABB(const glm::vec2& point, const glm::vec2& aabbMin, const glm::vec2& aabbMax);
		static void ResolveBoundaryCollision(glm::vec2& position, glm::vec2& velocity, float radius,
			const glm::vec2& boundMin, const glm::vec2& boundMax, float damping = 0.7f);

		// 单个粒子场的核函数
		static float SmoothingKernel(float radius, float dst)
		{
			if (dst >= radius) return 0.0f;
			float value = radius * radius - dst * dst;
			float spikyScalingFactor = 30.0f / (glm::pi<float>() * std::pow(radius, 5.0f));
			return value * value * value * spikyScalingFactor;
		}

		// 粒子场梯度的核函数（Spiky Kernel 的梯度）
		static float SmoothingKernelDerivative(float radius, float dst) {
			if (dst >= radius || dst <= 1e-5f) return 0.0f;

			float v = radius - dst;
			// 2D Spiky Kernel
			float spikyScalingFactor = 30.0f / (glm::pi<float>() * std::pow(radius, 5.0f));
			return v * v * spikyScalingFactor;
		}

		// 计算单个粒子场密度
		static float CalculateDensity(const Particle2D& sampleParticle, const std::vector<Particle2D>& particles,
			float smoothingRadius)
		{
			float density = 0.0f;
			const float mass = 1.0f;
			for (const Particle2D& particle : particles)
			{
				float dst = glm::distance(particle.Position, sampleParticle.Position);
				float influence = SmoothingKernel(smoothingRadius, dst);
				density += mass * influence;
			}
			return density;
		}

		// 计算粒子属性变动微分
		static float CalculateProperty(const Particle2D& sampleParticle, const std::vector<Particle2D>& particles,
			float smoothingRadius)
		{
			float property = 0.0f;
			const float mass = 1.0f;

			for (const Particle2D& particle : particles)
			{
				float dst = glm::distance(particle.Position, sampleParticle.Position);
				float influence = SmoothingKernel(smoothingRadius, dst);
				float density = CalculateDensity(sampleParticle, particles, smoothingRadius);
				property += particle.Property * influence * mass / density;

			}
			return property;
		}

		// 计算粒子场梯度
		static glm::vec2 CalculateGradient(const Particle2D& sampleParticle, const std::vector<Particle2D>& particles,
			float smoothingRadius)
		{
			glm::vec2 gradient(0.0f);
			const float mass = 1.0f;

			for (const Particle2D& particle : particles)
			{
				float dst = glm::distance(sampleParticle.Position, particle.Position);
				if (dst >= smoothingRadius || dst < 0.0001f) continue;
				float slope = SmoothingKernelDerivative(smoothingRadius, dst);
				glm::vec2 dir = (sampleParticle.Position - particle.Position) / dst;
				gradient += (particle.Property * mass / particle.Density) * (dir * slope);
			}

			return gradient;
		}

		// 计算和圆形障碍物碰撞的效果
		static void ResolveCircleCollision(glm::vec2& position, glm::vec2& velocity, float pRadius,
			const glm::vec2& circleCenter, float circleRadius, float damping);

	};

}
