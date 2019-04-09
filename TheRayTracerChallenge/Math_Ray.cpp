#include "stdafx.h"
#include "Math_Ray.h"
#include "Math_Transform.h"
#include "Math_Tuple.h"
#include "Math_Matrix.h"
#include "Graphics.h"

#include <cmath>

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

using H = Math::Helpers;
using C = Math::Helpers::Coordinate;
using CI = Math::Helpers::ColorInput;

namespace Math
{

}


#pragma region tests here
#ifdef _MSC_VER
namespace Math
{
	TEST_CLASS(TestMathRay)
	{
	public:
		TEST_METHOD(Ray_Creation)
		{
			auto point = H::MakePoint<float>(1.0f, 2.0f, 3.0f);
			auto vector = H::MakeVector<float>(4.0f, 5.0f, 6.0f);

			Ray ray{ point, vector };

			Assert::IsTrue(ray.GetOrigin() == point);
			Assert::IsTrue(ray.GetDirection() == vector);
		}

		TEST_METHOD(Ray_GetPosition)
		{
			auto point = H::MakePoint<float>(2.0f, 3.0f, 4.0f);
			auto vector = H::MakeVector<float>(1.0f, 0.0f, 0.0f);
			Ray ray{ point, vector };


			auto position0 = ray.GetPosition(0.0f);
			auto position1 = ray.GetPosition(1.0f);
			auto position2 = ray.GetPosition(-1.0f);
			auto position3 = ray.GetPosition(2.5f);

			Assert::IsTrue(position0 == H::MakePoint<float>(2.0f, 3.0f, 4.0f));
			Assert::IsTrue(position1 == H::MakePoint<float>(3.0f, 3.0f, 4.0f));
			Assert::IsTrue(position2 == H::MakePoint<float>(1.0f, 3.0f, 4.0f));
			Assert::IsTrue(position3 == H::MakePoint<float>(4.5f, 3.0f, 4.0f));
		}
	};
}

#endif
#pragma endregion