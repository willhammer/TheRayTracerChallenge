#include "stdafx.h"
#include "Math_Materials.h"
#include "Math_Ray.h"
#include "Graphics.h"
#include "Math_Primitives.h"
#include "Math_Tuple.h"
#include "Math_Matrix.h"

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
		Math::IMaterial<T>* material,
		const Math::ILight<T>& light,
		const Math::Point4<T>& eyePosition,
		const Math::Vector4<T>& eyeOrientation
	)
	{
		Math::PhongMaterial<T>* materialAsPhong = dynamic_cast<Math::PhongMaterial<T>*>(material);
		if (materialAsPhong == nullptr)
			return H::MakeColor(0.0f, 0.0f, 0.0f, 0.5f);

		auto effective_color = materialAsPhong->GetColor() * light.GetIntensity();
		auto pointToLightDirection = light.GetPosition() - point;
		pointToLightDirection.Normalize();

		auto ambient = effective_color * materialAsPhong->GetValue(Math::PhongValueType::Ambient);		
		auto diffuse = H::MakeColor<T>(0.0f, 0.0f, 0.0f);
		auto specular = H::MakeColor<T>(0.0f, 0.0f, 0.0f);
		
		auto light_dot_normal = pointToLightDirection.Dot(surfaceNormal);
		if (light_dot_normal >= 0.0f)
		{
			diffuse = effective_color * materialAsPhong->GetValue(Math::PhongValueType::Diffuse) * light_dot_normal;
			auto reflectionVector = Reflect(pointToLightDirection * -1.0f, surfaceNormal);
			auto reflection_dot_eye = reflectionVector.Dot(eyeOrientation * -1.0f);
			if (reflection_dot_eye > 0.0f)
			{
				auto specularFactor = (T)pow(reflection_dot_eye, materialAsPhong->GetValue(Math::PhongValueType::Shininess));
				specular = light.GetIntensity() * materialAsPhong->GetValue(Math::PhongValueType::Specular) * specularFactor;
			}
		}

		return ambient + diffuse + specular;
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

		TEST_METHOD(PhongColor_Light_EyeBetweenLightAndPoint)
		{
			auto eyePosition = H::MakePoint<float>(0.0f, 0.0f, -1.0f);
			auto eyeOrientation = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
			
			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto normalAtPoint = H::MakeVector<float>(0.0f, 0.0f, -1.0f);

			auto light = LightOmni<float>();
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			light.SetPosition(H::MakePoint<float>(0.0f, 0.0f, -10.0f));

			auto material = PhongMaterial<float>::GetDefaultMaterial();
			Assert::IsTrue(GetColorOnMaterialAtPoint<float>(point, normalAtPoint, material, light, eyePosition, eyeOrientation) == H::MakeColor<float>(1.9f, 1.9f, 1.9f));		
		}

		TEST_METHOD(PhongColor_Light_EyeAt45Angle)
		{
			static const float sqrt2Over2 = sqrt(2.0f) / 2.0f;
			auto eyePosition = H::MakePoint<float>(0.0f, -sqrt2Over2, sqrt2Over2);
			auto eyeOrientation = H::MakeVector<float>(0.0f, sqrt2Over2, -sqrt2Over2);

			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto normalAtPoint = H::MakeVector<float>(0.0f, 0.0f, -1.0f);

			auto light = LightOmni<float>();
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			light.SetPosition(H::MakePoint<float>(0.0f, 0.0f, -10.0f));

			auto material = PhongMaterial<float>::GetDefaultMaterial();
			Assert::IsTrue(GetColorOnMaterialAtPoint<float>(point, normalAtPoint, material, light, eyePosition, eyeOrientation) == H::MakeColor<float>(1.0f, 1.0f, 1.0f));
		}

		TEST_METHOD(PhongColor_Light_LightAt45Angle)
		{	
			auto eyePosition = H::MakePoint<float>(0.0f, 0.0f, -1.0f);
			auto eyeOrientation = H::MakeVector<float>(0.0f, 0.0f, 1.0f);

			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto normalAtPoint = H::MakeVector<float>(0.0f, 0.0f, -1.0f);

			auto light = LightOmni<float>();
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			light.SetPosition(H::MakePoint<float>(0.0f, 10.0f, -10.0f));

			auto material = PhongMaterial<float>::GetDefaultMaterial();
			auto colorOnMaterialAtPoint = GetColorOnMaterialAtPoint<float>(point, normalAtPoint, material, light, eyePosition, eyeOrientation);
			Assert::IsTrue(colorOnMaterialAtPoint == H::MakeColor<float>(0.736396074f, 0.736396074f, 0.736396074f));
		}

		TEST_METHOD(PhongColor_Light_LightAt45Angle_And_EyeAt45Angle)
		{
			static const float sqrt2Over2 = sqrt(2.0f) / 2.0f;
			auto eyePosition = H::MakePoint<float>(0.0f, -sqrt2Over2, -sqrt2Over2);
			auto eyeOrientation = H::MakeVector<float>(0.0f, sqrt2Over2, sqrt2Over2);

			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto normalAtPoint = H::MakeVector<float>(0.0f, 0.0f, -1.0f);

			auto light = LightOmni<float>();
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			light.SetPosition(H::MakePoint<float>(0.0f, 10.0f, -10.0f));

			auto material = PhongMaterial<float>::GetDefaultMaterial();
			auto colorOnMaterialAtPoint = GetColorOnMaterialAtPoint<float>(point, normalAtPoint, material, light, eyePosition, eyeOrientation);
			Assert::IsTrue(colorOnMaterialAtPoint == H::MakeColor<float>(1.63638532f, 1.63638532f, 1.63638532f));
		}

		TEST_METHOD(PhongColor_Light_LightBehindPoint)
		{
			static const float sqrt2Over2 = sqrt(2.0f) / 2.0f;
			auto eyePosition = H::MakePoint<float>(0.0f, 0.0f, -1.0f);
			auto eyeOrientation = H::MakeVector<float>(0.0f, 0.0f, 1.0f);

			auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
			auto normalAtPoint = H::MakeVector<float>(0.0f, 0.0f, -1.0f);

			auto light = LightOmni<float>();
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			light.SetPosition(H::MakePoint<float>(0.0f, 0.0f, 10.0f));

			auto material = PhongMaterial<float>::GetDefaultMaterial();
			Assert::IsTrue(GetColorOnMaterialAtPoint<float>(point, normalAtPoint, material, light, eyePosition, eyeOrientation) == H::MakeColor<float>(0.1f, 0.1f, 0.1f));
		}

		TEST_METHOD(Phong_Color_Sphere)
		{
			Sphere<float> sphere;
			sphere.SetPosition(H::MakePoint(100.0f, 150.0f, 0.0f));
			sphere.SetRadius(80.0f);

			sphere.SetMaterial(PhongMaterial<float>::GetDefaultMaterial());
			sphere.GetMaterial()->SetColor(H::MakeColor<float>(1.0f, 0.2f, 1.0f));

			LightOmni<float> light;
			light.SetPosition(H::MakePoint<float>(-10.0f, -10.0f, -100.0f));
			light.SetIntensity(H::MakeColor<float>(1.0f, 1.0f, 1.0f));
			
			auto eyePosition = sphere.GetPosition() - H::MakeVector<float>(0.0f, 0.0f, 1000.0f);
			auto eyeOrientation = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
			
			std::vector<std::vector<Ray<float>>> rayMatrix;
			size_t width = 160 * 2;
			size_t height = 90 * 2;

			Graphics::Canvas canvas(width, height);
			
			Math::Vector4f rayDirection = H::MakeVector(0.0f, 0.0f, 1.0f);
			
			rayMatrix.resize(height);
			for (size_t i = 0; i < height; ++i)
			{
				rayMatrix[i].resize(width);
				for (size_t j = 0; j < width; ++j)
				{
					rayDirection = H::MakePoint(float(i), float(j), 0.0f) - eyePosition;
					rayDirection.Normalize();

					rayMatrix[i][j].SetOrigin(eyePosition);
					rayMatrix[i][j].SetDirection(rayDirection);

					eyeOrientation = rayDirection * 1.0f;
					
					auto hit = rayMatrix[i][j].Intersect(&sphere);
					if (hit.objectHits.size() > 0)
					{
						auto hitPosition = hit.objectHits[0];
						auto normal = sphere.GetNormalAtPoint(hitPosition);

						auto color = GetColorOnMaterialAtPoint(hitPosition, normal, sphere.GetMaterial().get(), light, eyePosition, eyeOrientation);

						auto posX = H::Get(hitPosition, C::X);
						auto posY = H::Get(hitPosition, C::Y);

						canvas.SetAt((size_t)posX, (size_t)posY, color);
					}
				}
			}

			canvas.SetFilename("sphereLighting.ppm");
			canvas.WritePPMFile();
		}
	};
}

#endif
#pragma endregion