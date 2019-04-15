#include "stdafx.h"
#include "Math_Primitives.h"
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
    size_t Object::objectIdCounter = 0;
    std::unordered_map<size_t, Object*> Object::objectMap = std::unordered_map<size_t, Object*>();
}

#pragma region tests here
#ifdef _MSC_VER
namespace Math
{
	TEST_CLASS(TestMathPrimitives_Object)
	{
	public:
		TEST_METHOD(Object_counting)
		{
			Object obj;
			Assert::IsTrue(obj.GetObjectId() == 1);
			Assert::IsTrue(Object::GetNumObjects() == 1);

			Object obj1;
			Assert::IsTrue(obj1.GetObjectId() == 2);
			Assert::IsTrue(Object::GetNumObjects() == 2);
			
			{
				Sphere<float> obj2;
				Assert::IsTrue(obj2.GetObjectId() == 3);
				Assert::IsTrue(Object::GetNumObjects() == 3);
			}

			Assert::IsTrue(Object::GetNumObjects() == 2);			
		}
	};
    
	TEST_CLASS(TestMathPrimitives_Sphere)
	{
	public:
		TEST_METHOD(Sphere_Creation)
		{
            Sphere<float> sphere;
            Assert::IsTrue(sphere.GetRadius() == 0.0f);
            Assert::IsTrue(sphere.GetPosition() == H::MakePoint(0.0f, 0.0f, 0.0f));

            Sphere<float> sphere2(15.0f, H::MakePoint(2.0f, 3.0f, 4.0f));
            Assert::IsTrue(sphere2.GetRadius() == 15.0f);
            Assert::IsTrue(sphere2.GetPosition() == H::MakePoint(2.0f, 3.0f, 4.0f));
		}

	};

	TEST_CLASS(TestMathPrimitives_Cube)
	{
	public:
		TEST_METHOD(Cube_Creation)
		{

		}
	};

}
#endif

#pragma endregion