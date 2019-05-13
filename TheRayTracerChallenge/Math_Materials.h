#pragma once


#include "stdafx.h"
#include "Math_Common.h"
#include "Math_Matrix.h"
#include "Math_Tuple.h"
#include "Math_Transform.h"
#include "Math_Ray.h"
#include <unordered_map>
#include <unordered_set>

namespace Math
{
	template<typename T>
	class IMaterial
	{
	private:
		Color4<T> color;
	public:
		void SetColor(const Color4<T>& setColor) { color = setColor; }
		Color4<T> GetColor() { return color; }
		virtual Ray<T> Reflect(const Ray<T>& ray) = 0;
	};

	template<typename T>
	class IRenderData
	{
	protected:
		Transform<T> transform;
		IMaterial<T>* material;

	public:
		virtual Vector4<T> GetNormalAtPoint(const Point4<T>& point) = 0;
		Transform<T> GetTransform() { return transform; }
		void SetTransform(const Transform<T>& setTransform) { transform = setTransform; }
		IRenderData() : material{ nullptr } { }
	};

	template<typename T>
	class PhongMaterial
	{
	private:
		T ambient;
		T diffuse;
		T specular;
		T shininess;

	public:
		Ray<T> Reflect(const Ray<T>& ray) override { return Ray<T>(); };
	};
}



