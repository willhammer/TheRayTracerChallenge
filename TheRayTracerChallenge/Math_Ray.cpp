#include "stdafx.h"
#include "Math_Ray.h"
#include "Math_Primitives.h"
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
	std::vector<T> IntersectSphere(Math::Ray<T> ray, Math::Sphere<T>* obj)
    {
        if (obj == nullptr)
            return std::vector<T>();

        static const auto pointZero = H::MakePoint<T>(T(0), T(0), T(0));        
        ray.Normalize();

		auto spherePosition = obj->GetPosition();

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
		
		obj->SetPosition(spherePosition);
        return SolveQuadratic(a, b, c);
    }
}


namespace Math
{
    template<typename T>
    RayHit<T> Ray<T>::Intersect(Object* obj)
    {
		RayHit<T> ray;
		std::vector<T> solutions;

		if (IsA(Sphere<T>*, decltype(obj)))
		{
			solutions = IntersectSphere(*this, reinterpret_cast<Sphere<T>*>(obj));
		}

		if (solutions.empty())
			return ray;

		for (auto& value : solutions)
		{
			auto objectHit = this->GetOrigin() + (this->GetDirection() * value);
			if (value >= T(0))
			{
				ray.hitDistances.push_back(value);
				ray.objectHits.push_back(objectHit);
			}
			else
			{
				ray.negativeHitDistances.push_back(value);
				ray.negativeObjectHits.push_back(objectHit);
			}
		}

        return ray;
    }

	template<typename T>	
	Ray<T> Ray<T>::Transform(Math::Transform<T>& transform)
	{
		Ray<T> newRay;
		newRay.SetOrigin(this->GetOrigin() * transform);
		newRay.SetDirection(this->GetDirection() * transform);

		return newRay;
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

        TEST_METHOD(Ray_SphereIntersection_ZeroPoints)
        {
            auto point = H::MakePoint<float>(0.0f, 2.0f, -5.0f);
            auto vector = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
            Ray<float> ray{ point, vector };

            Sphere<float> sphere;
            sphere.SetRadius(1);
            sphere.SetPosition(H::MakePoint<float>(0.0f, 0.0f, 0.0f));

            auto intersectionPoints = ray.Intersect(&sphere);
            Assert::IsTrue(intersectionPoints.objectHits.size() == 0);
        }

        TEST_METHOD(Ray_SphereIntersection_OnePoint)
        {
            auto point = H::MakePoint<float>(0.0f, 1.0f, -5.0f);
            auto vector = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
            Ray<float> ray{ point, vector };

            Sphere<float> sphere;
            sphere.SetRadius(1);
            sphere.SetPosition(H::MakePoint<float>(0.0f, 0.0f, 0.0f));

            auto intersectionPoints = ray.Intersect(&sphere);
            Assert::IsTrue(intersectionPoints.objectHits.size() == 1);
            Assert::IsTrue(intersectionPoints.objectHits.at(0) == H::MakePoint<float>(0.0f, 1.0f, 0.0f));
        }

        TEST_METHOD(Ray_SphereIntersection_OnePoint_FromInsideSphere)
        {
            auto point = H::MakePoint<float>(0.0f, 0.0f, 0.0f);
            auto vector = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
            Ray<float> ray{ point, vector };

            Sphere<float> sphere;
            sphere.SetRadius(1);
            sphere.SetPosition(H::MakePoint<float>(0.0f, 0.0f, 0.0f));

            auto intersectionPoints = ray.Intersect(&sphere);
            Assert::IsTrue(intersectionPoints.objectHits.size() == 1);
            Assert::IsTrue(intersectionPoints.objectHits.at(0) == H::MakePoint<float>(0.0f, 0.0f, 1.0f));
			Assert::IsTrue(intersectionPoints.negativeObjectHits.size() == 1);

        }

        TEST_METHOD(Ray_SphereIntersection_ZeroPoints_SphereBehindRay)
        {
            auto point = H::MakePoint<float>(0.0f, 0.0f, 5.0f);
            auto vector = H::MakeVector<float>(0.0f, 0.0f, 1.0f);
            Ray<float> ray{ point, vector };

            Sphere<float> sphere;
            sphere.SetRadius(1);
            sphere.SetPosition(H::MakePoint<float>(0.0f, 0.0f, 0.0f));

            auto intersectionPoints = ray.Intersect(&sphere);
            Assert::IsTrue(intersectionPoints.objectHits.size() == 0);
			Assert::IsTrue(intersectionPoints.negativeObjectHits.size() == 2);
        }

		TEST_METHOD(Ray_TransformRay)
		{
			auto point = H::MakePoint<float>(1.0f, 2.0f, 3.0f);
			auto vector = H::MakeVector<float>(0.0f, 1.0f, 0.0f);
			Ray<float> ray{ point, vector };

			Transform<float> transform = Transform<float>::MakeTranslation(3.0f, 4.0f, 5.0f);
			Transform<float> transform2 = Transform<float>::MakeScaling(2.0f, 3.0f, 4.0f);

			auto rayTransformed = ray.Transform(transform);
			Assert::IsTrue(rayTransformed.GetOrigin() == H::MakePoint<float>(4.0f, 6.0f, 8.0f));
			Assert::IsTrue(rayTransformed.GetDirection() == H::MakeVector<float>(0.0f, 1.0f, 0.0f));

			auto rayTransformed2 = ray.Transform(transform2);
			Assert::IsTrue(rayTransformed2.GetOrigin() == H::MakePoint<float>(2.0f, 6.0f, 12.0f));
			Assert::IsTrue(rayTransformed2.GetDirection() == H::MakeVector<float>(0.0f, 3.0f, 0.0f));
		}

		TEST_METHOD(Ray_SphereIntersection_DawingSphereShadowOnCanvas)
		{
			Color4f sphereShadowColor = H::MakeColor(1.0f, 0.0f, 0.0f, 0.5f);
			std::vector<std::vector<Ray<float>>> rayMatrix;
			
			size_t width = 16 * 20;
			size_t height = 9 * 20;

			Sphere<float> sphere;
			Graphics::Canvas canvas(width, height);

			sphere.SetPosition(H::MakePoint(100.0f, 100.0f, 0.0f));
			sphere.SetRadius(75.0f);

			const Math::Vector4f rayDirection = H::MakeVector(0.0f, 0.0f, 1.0f);
			rayMatrix.resize(height);

			for (size_t i = 0; i < height; ++i)
			{
				rayMatrix[i].resize(width);

				for (size_t j = 0; j < width; ++j)
				{
					rayMatrix[i][j].SetOrigin(H::MakePoint(float(i), float(j), -50.0f));
					rayMatrix[i][j].SetDirection(rayDirection);
					auto hit = rayMatrix[i][j].Intersect(&sphere);
					for (auto& hitPosition : hit.objectHits)
					{
						auto posX = H::Get(hitPosition, C::X);
						auto posY = H::Get(hitPosition, C::Y);

						canvas.SetAt((size_t)posY, (size_t)posX, sphereShadowColor);
					}
				}
			}

			canvas.SetFilename("sphereShadow.ppm");
			canvas.WritePPMFile();
		}
	};
}

#endif
#pragma endregion