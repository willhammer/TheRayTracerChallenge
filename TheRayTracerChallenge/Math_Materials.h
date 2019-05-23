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
	protected:
		Point4<T> position;
		Color4<T> intensity;

	public:
		void SetPosition(const Point4<T>& setPosition) { position = setPosition; }
		Point4<T> GetPosition() const { return position; }

		void SetIntensity(const Color4<T>& setIntensity) { intensity = setIntensity; }
		Color4<T> GetIntensity() const { return intensity; }
	};


	template<typename T>
	class LightOmni : public ILight<T>
	{
	public:

		LightOmni()
		{
			SetPosition(H::MakePoint<T>(0.0f, 0.0f, 0.0f));
			SetIntensity(H::MakeColor<T>(0.0f, 0.0f, 0.0f, 0.5f));
		}

		LightOmni(const Point4<T>& setPosition, const Color4<T>& setIntensity)
		{
			SetPosition(setPosition);
			SetIntensity(setIntensity); 
		}
	};

	template<typename T>
	class IMaterial
	{
	private:
		Color4<T> color;
	public:
		void SetColor(const Color4<T>& setColor) { color = setColor; }
		Color4<T> GetColor() const { return color; }
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
		Transform<T> GetTransform() const { return transform; }
		void SetTransform(const Transform<T>& setTransform) { transform = setTransform; }	

		void SetMaterial(IMaterial<T>* setMaterial) { material.reset(setMaterial); }
		std::shared_ptr<IMaterial<T>> GetMaterial() const { return material; }
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

		static PhongMaterial<T>* GetDefaultMaterial()
		{
			PhongMaterial<T>* material = new PhongMaterial<T>();
			material->SetColor(H::MakeColor<T>((T)1.0, (T)1.0, (T)1.0, (T)0.5));
			material->SetValue(PhongValueType::Ambient, (T)0.1);
			material->SetValue(PhongValueType::Diffuse, (T)0.9);
			material->SetValue(PhongValueType::Specular, (T)0.9);
			material->SetValue(PhongValueType::Shininess, (T)200.0);

			return material;
		}
	};
}



