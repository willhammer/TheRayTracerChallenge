#include "stdafx.h"
#include "Math_Primitives.h"

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



namespace Math
{
    size_t Object::objectIdCounter = 0;
    std::unordered_map<size_t, Object*> Object::objectMap = std::unordered_map<size_t, Object*>();
    
    template<typename T>
    Vector4<T> Sphere<T>::GetNormalAtPoint(const Point4<T>& point)
    {
        static auto identity = Transform<T>::Identity();
        auto transform = this->GetTransform().GetRotation();        
        auto inverse = transform.GetInverse();

        auto pointLocalSpace = point * inverse;
        auto thisPositionLocalSpace = this->GetPosition() * inverse;
        auto normalLocalSpace = pointLocalSpace - thisPositionLocalSpace;
        
        inverse.SetTransposed(true);
        auto normalWorldSpace = normalLocalSpace * inverse;
        normalWorldSpace.Normalize();

        return H::MakeVector(H::Get(normalWorldSpace, C::X), H::Get(normalWorldSpace, C::Y), H::Get(normalWorldSpace, C::Z));
    }
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
			Object::ResetObjectMap();

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
        
		TEST_METHOD(Sphere_Material)
		{
			Sphere<float> sphere;
			sphere.SetMaterial(new PhongMaterial<float>());
			auto material = sphere.GetMaterial();
			auto materialAsPhong = dynamic_cast<PhongMaterial<float>*>(material.get());
			Assert::IsTrue(materialAsPhong != nullptr);
			Assert::IsTrue(materialAsPhong->GetValue(PhongValueType::Ambient) == 0.0f);
		}

        TEST_METHOD(Sphere_Normal)
        {
            Sphere<float> sphere(1.0f, H::MakePoint(0.0f, 0.0f, 0.0f));
            auto point = H::MakePoint(1.0f, 0.0f, 0.0f);

            auto normalVec = sphere.GetNormalAtPoint(point);
            Assert::IsTrue(normalVec == H::MakeVector(1.0f, 0.0f, 0.0f));

            auto point2 = H::MakePoint(0.0f, 1.0f, 0.0f);
            auto normalVec2 = sphere.GetNormalAtPoint(point2);
            Assert::IsTrue(normalVec2 == H::MakeVector(0.0f, 1.0f, 0.0f));

            auto point3 = H::MakePoint(0.0f, 0.0f, 1.0f);
            auto normalVec3 = sphere.GetNormalAtPoint(point3);
            Assert::IsTrue(normalVec3 == H::MakeVector(0.0f, 0.0f, 1.0f));

            const float sqrt3Over3 = sqrt(3.0f) / 3.0f;
            auto point4 = H::MakePoint(sqrt3Over3, sqrt3Over3, sqrt3Over3);
            auto normalVec4 = sphere.GetNormalAtPoint(point4);
            Assert::IsTrue(normalVec4 == H::MakeVector(sqrt3Over3, sqrt3Over3, sqrt3Over3));

            Sphere<float> sphere2(1.0f, H::MakePoint(0.0f, 1.0f, 0.0f));
            auto point5 = H::MakePoint(0.0f, 1.70711f, -0.70711f);
            auto normalVec5 = sphere2.GetNormalAtPoint(point5);
            Assert::IsTrue(normalVec5 == H::MakeVector(0.0f, 0.707106829f, -0.707106829f));

            const float radZ = Math::GetPi<float>() / 5.0f;

            Sphere<float> sphere3(1.0f, H::MakePoint(0.0f, 0.0f, 0.0f));
            
            auto scaling = Transform<float>::MakeScaling(1.0f, 0.5f, 1.0f);
            auto rotation = Transform<float>::MakeRotation(0.0f, 0.0f, radZ);
            Transform<float> transform = scaling * rotation;
            sphere3.SetTransform(transform);

            const float sqrt2Over2 = sqrt(2.0f) / 2.0f;
            auto point6 = H::MakePoint(0.0f, sqrt2Over2, -sqrt2Over2);
            auto normalVec6 = sphere3.GetNormalAtPoint(point6);
            
            Assert::IsTrue(normalVec6 == H::MakeVector(0.0f, 0.970142543f, -0.242535636f));
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