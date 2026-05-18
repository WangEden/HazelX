#include "hzpch.h"
#include "Physics2D.h"

namespace Hazel {

	bool Physics2D::PointInAABB(const glm::vec2& point, const glm::vec2& aabbMin, const glm::vec2& aabbMax)
	{
		return point.x >= aabbMin.x && point.x <= aabbMax.x &&
			point.y >= aabbMin.y && point.y <= aabbMax.y;
	}

	void Physics2D::ResolveBoundaryCollision(glm::vec2& position, glm::vec2& velocity, float radius,
		const glm::vec2& boundMin, const glm::vec2& boundMax, float damping)
	{
		// X轴边界
		if (position.x - radius < boundMin.x) {
			position.x = boundMin.x + radius;
			velocity.x = -velocity.x * damping;
		}
		else if (position.x + radius > boundMax.x) {
			position.x = boundMax.x - radius;
			velocity.x = -velocity.x * damping;
		}

		// Y轴边界
		if (position.y - radius < boundMin.y) {
			position.y = boundMin.y + radius;
			velocity.y = -velocity.y * damping;
		}
		else if (position.y + radius > boundMax.y) {
			position.y = boundMax.y - radius;
			velocity.y = -velocity.y * damping;
		}
	}

	void Physics2D::ResolveCircleCollision(glm::vec2& position, glm::vec2& velocity, float pRadius,
		const glm::vec2& circleCenter, float circleRadius, float damping)
	{
		float minDst = pRadius + circleRadius;
		float dist = glm::distance(position, circleCenter);

		if (dist < minDst)
		{
			glm::vec2 normal = glm::normalize(position - circleCenter);

			position = circleCenter + normal * minDst;

			float dot = glm::dot(velocity, normal);
			if (dot < 0) // 只有当粒子向圆心运动时才反弹
			{
				velocity = (velocity - 2.0f * dot * normal) * damping;
			}
		}
	}

}
