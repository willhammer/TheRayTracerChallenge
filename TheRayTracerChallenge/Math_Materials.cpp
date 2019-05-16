#include "stdafx.h"
#include "Math_Materials.h"
#include "Math_Ray.h"
#include "Graphics.h"

#include <cmath>

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

using H = Math::Helpers;
using C = Math::Helpers::Coordinate;
using CI = Math::Helpers::ColorInput;

namespace
{
	template<typename T>
	Math::Vector4<T> Reflect(const Math::Vector4<T>& vector, const Math::Vector4<T>& normal)
	{
		return vector - normal * (T(2) * vector.Dot(normal));
	}

	template<typename T>
	Math::Color4<T> GetColorOnMaterialAtPoint
	(
		const Math::Point4<T>& point, 
		const Math::Vector4<T>& surfaceNormal,
		const Math::IMaterial<T>& material,
		const Math::ILight<T>& light,
		const Math::Point4<T>& eyePosition,
		const Math::Vector4<T>& eyeOrientation
	)
	{
		return H::MakeColor<T>(T(0), T(0), T(0), T(0));
	}


}

namespace Math
{
	


}


#pragma region tests here
#ifdef _MSC_VER
namespace Math
{

	TEST_CLASS(TestMaterials)
	{
	public:
		TEST_METHOD(Ray_Reflect)
		{
			static const float sqrt2Over2 = sqrt(2.0f) / 2.0f;

			Vector4f vector = H::MakeVector<float>(1.0f, -1.0f, 0.0f);
			Vector4f normal = H::MakeVector<float>(0.0f, 1.0f, 0.0f);

			auto vectorReflected = Reflect<float>(vector, normal);
			Assert::IsTrue(vectorReflected == H::MakeVector(1.0f, 1.0f, 0.0f));

			vector = H::MakeVector<float>(0.0f, -1.0f, 0.0f);
			normal = H::MakeVector<float>(sqrt2Over2, sqrt2Over2, 0.0f);
			vectorReflected = Reflect<float>(vector, normal);
			Assert::IsTrue(vectorReflected == H::MakeVector(1.0f, 0.0f, 0.0f));
		}

		TEST_METHOD(Light)
		{
			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto color = H::MakeColor<float>(1.0f, 1.0f, 1.0f, 0.5f);

			LightOmni<float> omni(point, color);

			Assert::IsTrue(omni.GetIntensity() == color);
			Assert::IsTrue(omni.GetPosition() == point);
		}

		TEST_METHOD(Material)
		{
			PhongMaterial<float> material1{ 0.1f, 0.9f, 0.9f, 200.0f };
			PhongMaterial<float> material2;

			material2.SetValue(PhongValueType::Ambient, 0.1f);
			material2.SetValue(PhongValueType::Diffuse, 0.9f);
			material2.SetValue(PhongValueType::Specular, 0.9f);
			material2.SetValue(PhongValueType::Shininess, 200.0f);

			Assert::IsTrue(material1.GetValue(PhongValueType::Ambient) == 0.1f);
			Assert::IsTrue(material2.GetValue(PhongValueType::Ambient) == 0.1f);

			Assert::IsTrue(material1.GetValue(PhongValueType::Diffuse) == 0.9f);
			Assert::IsTrue(material2.GetValue(PhongValueType::Diffuse) == 0.9f);

			Assert::IsTrue(material1.GetValue(PhongValueType::Specular) == 0.9f);
			Assert::IsTrue(material2.GetValue(PhongValueType::Specular) == 0.9f);

			Assert::IsTrue(material1.GetValue(PhongValueType::Shininess) == 200.0f);
			Assert::IsTrue(material2.GetValue(PhongValueType::Shininess) == 200.0f);
		}

		TEST_METHOD(Material_Color_With_Light)
		{
			auto eyePosition = H::MakePoint<float>(0.0f, 0.0f, -1.0f);
			auto eyeOrientation = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
			
			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto normalAtPoint = H::MakeVector<float>(0.0f, 0.0f, -1.0f);

			auto light = LightOmni<float>();
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			light.SetPosition(H::MakePoint<float>(0.0f, 0.0f, -10.0f));

			auto material = PhongMaterial<float>();

			Assert::IsTrue(GetColorOnMaterialAtPoint<float>(point, normalAtPoint, material, light, eyePosition, eyeOrientation) == H::MakeColor<float>(1.9f, 1.9f, 1.9f));
		}
	};
}

#endif
#pragma endregion