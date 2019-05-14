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
	class ILight
	{
	private:
		Point4<T> position;
		T intensity;
	};

	template<typename T>
	class IMaterial
	{
	private:
		Color4<T> color;
	public:
		void SetColor(const Color4<T>& setColor) { color = setColor; }
		Color4<T> GetColor() { return color; }
		virtual Ray<T> Reflect(const Ray<T>& ray) { return Ray<T>(); }
	};

	template<typename T>
	class IRenderData
	{
	protected:
		Transform<T> transform;
		std::shared_ptr<IMaterial<T>> material;

	public:
		virtual Vector4<T> GetNormalAtPoint(const Point4<T>& point) = 0;
		Transform<T> GetTransform() { return transform; }
		void SetTransform(const Transform<T>& setTransform) { transform = setTransform; }	

		void SetMaterial(IMaterial<T>* setMaterial) { material.reset(setMaterial); }
		std::shared_ptr<IMaterial<T>> GetMaterial() { return material; }		
	};

	enum class PhongValueType : int
	{
		Ambient = 0,
		Diffuse = 1,
		Specular = 2,
		Shininess = 3
	};

	template<typename T>
	class PhongMaterial : public IMaterial<T>
	{
	private:
		T ambient;
		T diffuse;
		T specular;
		T shininess;

		T& GetValueByType(const PhongValueType valueType)
		{
			switch (valueType)
			{
			case PhongValueType::Ambient:
				return ambient;

			case PhongValueType::Diffuse:
				return diffuse;

			case PhongValueType::Specular:
				return specular;

			case PhongValueType::Shininess:
				return shininess;

			default:
				return ambient;
			}
		}

	public:
		PhongMaterial() : 
			ambient{T(0)},
			diffuse{T(0)},
			specular{T(0)},
			shininess{T(0)}
		{

		}

		PhongMaterial(T setAmbient, T setDiffuse, T setSpecular, T setShininess) :
			ambient{setAmbient},
			diffuse{setDiffuse},
			specular{setSpecular},
			shininess{setShininess}
		{

		}

		void SetValue(const PhongValueType valueType, T value)
		{
			GetValueByType(valueType) = value;
		}

		T GetValue(const PhongValueType valueType)
		{
			return GetValueByType(valueType);
		}
	};
}



