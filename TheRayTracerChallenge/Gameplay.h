#pragma once

#include <type_traits>
#include <limits>
#include <variant>

#include "Math.h"


using Point4f = Math::Point4<float>;
using Vector4f = Math::Vector4<float>;
using Tuple4f = Math::Tuple4<float>;

namespace Gameplay
{
	class Environment
	{
	public:
		Vector4f gravity;
		Vector4f wind;
	};

	class Projectile
	{
	public:
		Point4f position;
		Vector4f velocity;

		void Tick(const Environment& env, const Vector4f extraVelocity);
	};
};

