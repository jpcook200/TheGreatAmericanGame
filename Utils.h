#pragma once
#include "Vec2.h"
#include <numbers>
#include <cmath>

namespace utils 
{
	inline float angleBetween(Vec2& pointOne, Vec2& pointTwo)
	{
		Vec2 dir = pointTwo - pointOne;

		return std::atan2(dir.y, dir.x) * 180.f / std::numbers::pi_v<float>;
	}
}
