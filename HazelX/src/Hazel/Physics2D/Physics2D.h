#pragma once

#include <glm/glm.hpp>
#include <algorithm>

#define PI 3.141593

namespace Hazel {

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
	};

}
