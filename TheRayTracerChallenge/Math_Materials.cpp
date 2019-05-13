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
}

#pragma region tests here
#ifdef _MSC_VER
namespace Math
{

	TEST_CLASS(TestReflect)
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
	};
}

#endif
#pragma endregion