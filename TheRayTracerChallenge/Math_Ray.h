#pragma once


#include "stdafx.h"
#include "Math_Common.h"
#include "Math_Matrix.h"
#include "Math_Tuple.h"
#include <vector>
#include <map>
#include <unordered_map>

namespace Math
{
    class Object;

	template<typename T>
	class Ray
	{
	protected:
		Point4<T> origin;
		Vector4<T> direction;

	public:		
		Ray()
		{
			origin = H::MakePoint(T(0), T(0), T(0));
			direction = H::MakeVector(T(0), T(0), T(0));
		}
		
		Ray(const Point4<T>& setOrigin, const Vector4<T>& setDirection) :
			origin(setOrigin),
			direction(setDirection)
		{

		}

		Point4<T>& GetOrigin() { return origin; }
		Vector4<T>& GetDirection() { return direction; }
		Point4<T> GetPosition(T time) { return origin + direction * time; }
        void SetOrigin(const Point4<T>& setOrigin) { origin = setOrigin; }
        void SetDirection(const Vector4<T>& setDirection) { direction = setDirection; }

        void Normalize() { direction.Normalize(); }

        std::vector<Point4<T>> Intersect(Object* obj);

	};
}