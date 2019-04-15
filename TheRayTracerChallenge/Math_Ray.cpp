#include "stdafx.h"
#include "Math_Ray.h"
#include "Math_Primitives.h"
#include "Math_Transform.h"
#include "Math_Tuple.h"
#include "Math_Matrix.h"
#include "Graphics.h"

#include <vector>
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
    std::vector<T> SolveQuadratic(const T &a, const T &b, const T &c)
    {
        std::vector<T> solutions;

        T discr = b * b - T(4) * a * c;
        if (discr < 0) 
            return solutions;

        else if (discr == 0) 
            solutions.push_back(T(-0.5) * b / a);

        else 
        {
            T q = (b > 0) 
                ? T(-0.5) * (b + sqrt(discr)) 
                : T(-0.5) * (b - sqrt(discr));

            auto x0 = q / a;
            auto x1 = c / q;
            if (x0 > x1) 
                std::swap(x0, x1);

            solutions.push_back(x0);
            solutions.push_back(x1);
        }

        return solutions;
    }

    template<typename T>
    std::vector<Math::Point4<T>> IntersectSphere(Math::Ray<T> ray, Math::Sphere<T>* obj)
    {
        std::vector<Math::Point4<T>> intersectionPoints;
        std::vector<T> solutions;
        if (obj == nullptr)
            return intersectionPoints;

        static const auto pointZero = H::MakePoint<T>(T(0), T(0), T(0));        
        ray.Normalize();

        if (!(obj->GetPosition() == pointZero))
        {
            auto positionDiff = obj->GetPosition() - pointZero;
            obj->SetPosition(obj->GetPosition() - positionDiff);
            ray.SetOrigin(ray.GetOrigin() - positionDiff);
        }

        Math::Vector4<T> centerToRayOrigin = ray.GetOrigin() - obj->GetPosition();
        auto dir = ray.GetDirection();
        auto radiusSquared = obj->GetRadius() * obj->GetRadius();

        float a = dir.GetMagnitudeSquared();
        float b = 2 * dir.Dot(centerToRayOrigin);
        float c = centerToRayOrigin.GetMagnitudeSquared() - radiusSquared;
        if (c > 0)
            return intersectionPoints;

        solutions = SolveQuadratic(a, b, c);
        if (solutions.empty())
            return intersectionPoints;

        for (auto& value : solutions)
        {
            if (value >= T(0))
            {
                intersectionPoints.push_back(ray.GetOrigin() + (ray.GetDirection() * value));
            }
        }

        return intersectionPoints;
    }
}


namespace Math
{
    template<typename T>
    std::vector<Point4<T>> Ray<T>::Intersect(Object* obj)
    {
        if (std::is_base_of_v<Sphere<T>, Object>) return IntersectSphere(*this, reinterpret_cast<Sphere<T>*>(obj));
        return std::vector<Point4<T>>();
    }
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

			Ray<float> ray{ point, vector };

			Assert::IsTrue(ray.GetOrigin() == point);
			Assert::IsTrue(ray.GetDirection() == vector);
		}

		TEST_METHOD(Ray_GetPosition)
		{
			auto point = H::MakePoint<float>(2.0f, 3.0f, 4.0f);
			auto vector = H::MakeVector<float>(1.0f, 0.0f, 0.0f);
			Ray<float> ray{ point, vector };
            
			auto position0 = ray.GetPosition(0.0f);
			auto position1 = ray.GetPosition(1.0f);
			auto position2 = ray.GetPosition(-1.0f);
			auto position3 = ray.GetPosition(2.5f);

			Assert::IsTrue(position0 == H::MakePoint<float>(2.0f, 3.0f, 4.0f));
			Assert::IsTrue(position1 == H::MakePoint<float>(3.0f, 3.0f, 4.0f));
			Assert::IsTrue(position2 == H::MakePoint<float>(1.0f, 3.0f, 4.0f));
			Assert::IsTrue(position3 == H::MakePoint<float>(4.5f, 3.0f, 4.0f));
		}

        TEST_METHOD(Ray_SphereIntersection)
        {
            auto point = H::MakePoint<float>(0.0f, 0.0f, -5.0f);
            auto vector = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
            Ray<float> ray{ point, vector };

            Sphere<float> sphere;
            sphere.SetRadius(1);
            sphere.SetPosition(H::MakePoint<float>(0.0f, 0.0f, 0.0f));

            auto intersectionPoints = ray.Intersect(&sphere);
            Assert::IsTrue(intersectionPoints.size() == 1);
            Assert::IsTrue(intersectionPoints.at(0) == H::MakePoint<float>(0.0f, 0.0f, 1.0f));
        }
	};
}

#endif
#pragma endregion