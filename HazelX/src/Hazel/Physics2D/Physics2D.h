#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#define PI 3.141593

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
		// 基础 AABB 检测
		static bool PointInAABB(const glm::vec2& point, const glm::vec2& aabbMin, const glm::vec2& aabbMax);

		// SPH 专用的边界约束：当粒子超出边界时，反弹并损失一定速度
		// boundMin/Max: 边界框
		// position: 粒子当前位置 (引用传入，直接修改)
		// velocity: 粒子当前速度 (引用传入，直接修改)
		// radius: 粒子半径 (用于计算真实碰撞点)
		// damping: 碰撞动能损耗 (如 0.7f 代表反弹后保留 70% 速度)
		static void ResolveBoundaryCollision(glm::vec2& position, glm::vec2& velocity, float radius,
			const glm::vec2& boundMin, const glm::vec2& boundMax, float damping = 0.7f);

		// 单个粒子场的核函数
		static float SmoothingKernel(float radius, float dst, float scalingFactor)
		{
			// 提前裁剪：如果距离大于平滑半径，影响严格为 0
			if (dst >= radius) return 0.0f;

			float value = radius * radius - dst * dst;
			// 省去移动函数的体积计算，用提前算好的值带入
			return value * value * value * scalingFactor; 
		}

		// 粒子场梯度的核函数（Spiky Kernel 的梯度）
		static float SmoothingKernelDerivative(float radius, float dst) {
			if (dst >= radius || dst <= 0.0f) return 0.0f;
			float v = radius - dst;
			return -v * v * 12.0f / (pow(radius, 4.0f) * PI); // 简化示例：Spiky kernel 导数
		}

		// 计算单个粒子场密度
		static float CalculateDensity(const Particle2D& sampleParticle, const std::vector<Particle2D>& particles,
			float smoothingRadius, float poly6ScalingFactor)
		{
			float density = 0.0f;
			const float mass = 1.0f; // 假设每个粒子的质量为 1
			for (const Particle2D& particle : particles)
			{
				// 两点间的直线距离
				float dst = glm::distance(particle.Position, sampleParticle.Position);
				float influence = SmoothingKernel(smoothingRadius, dst, poly6ScalingFactor);
				density += mass * influence;
			}
			return density;
		}

		// 计算粒子属性变动微分
		static float CalculateProperty(const Particle2D& sampleParticle, const std::vector<Particle2D>& particles,
			float smoothingRadius, float poly6ScalingFactor)
		{
			float property = 0.0f;
			const float mass = 1.0f;

			for (const Particle2D& particle : particles)
			{
				float dst = glm::distance(particle.Position, sampleParticle.Position);
				float influence = SmoothingKernel(smoothingRadius, dst, poly6ScalingFactor);
				float density = CalculateDensity(sampleParticle, particles, smoothingRadius, poly6ScalingFactor);
				property += particle.Property * influence * mass / density; // 这里简单地用密度归一化属性值

			}
			return property;
		}

		// 计算粒子场梯度
		static glm::vec2 CalculateGradient(const Particle2D& sampleParticle, const std::vector<Particle2D>& particles,
			float smoothingRadius, float poly6ScalingFactor)
		{
			const float stepSize = 0.001f; // 用于数值微分的步长

			Particle2D samplePointChange = sampleParticle;
			samplePointChange.Position.x += stepSize;
			samplePointChange.Position.y += stepSize;

			float deltaX = CalculateProperty(samplePointChange, particles, smoothingRadius, poly6ScalingFactor) - CalculateProperty(sampleParticle, particles, smoothingRadius, poly6ScalingFactor);
			float deltaY = CalculateProperty(samplePointChange, particles, smoothingRadius, poly6ScalingFactor) - CalculateProperty(sampleParticle, particles, smoothingRadius, poly6ScalingFactor);

			return glm::vec2(deltaX / stepSize, deltaY / stepSize);
		}

	};

}
