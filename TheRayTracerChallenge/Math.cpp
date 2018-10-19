#include "stdafx.h"
#include "Math.h"

#ifdef _MSC_VER
    #include "CppUnitTest.h"
    using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif


namespace Math
{
    namespace M = Math;
    using H = Math::Helpers;
    using C = Math::Helpers::Coordinate;


    using Point4f = M::Point4<float>;
    using Vector4f = M::Vector4<float>;
    using Tuple4f = M::Tuple4<float>;

    template<typename T>
    constexpr M::Tuple4<T> AddTuples(const M::Tuple4<T>& first, const M::Tuple4<T>& second)
    {
        return M::Tuple4<T> {
                H::Get(first, C::X) + H::Get(second, C::X),
                H::Get(first, C::Y) + H::Get(second, C::Y),
                H::Get(first, C::Z) + H::Get(second, C::Z),
                H::Get(first, C::W) + H::Get(second, C::W) };
    }

    template<typename T>
    constexpr M::Tuple4<T> SubtractTuples(const M::Tuple4<T>& first, const M::Tuple4<T>& second)
    {
        return M::Tuple4<T> {
                H::Get(first, C::X) - H::Get(second, C::X),
                H::Get(first, C::Y) - H::Get(second, C::Y),
                H::Get(first, C::Z) - H::Get(second, C::Z),
                H::Get(first, C::W) - H::Get(second, C::W)};
    }

    template<typename T> M::Vector4<T> operator*(const M::Vector4<T>& vector, const T scalar)
    {
        return Helpers::MakeVector(
                H::Get(vector, C::X) * scalar,
                H::Get(vector, C::Y) * scalar,
                H::Get(vector, C::Z) * scalar);
    }

    template<typename T> M::Vector4<T> operator/(const M::Vector4<T>& vector, const T scalar)
    {
        return Helpers::MakeVector(
                H::Get(vector, C::X) / scalar,
                H::Get(vector, C::Y) / scalar,
                H::Get(vector, C::Z) / scalar);
    }

    template<typename T> void operator*= (M::Vector4<T>& vector, const T scalar)
    {
        H::Set(vector, C::X, H::Get(vector, C::X) * scalar);
        H::Set(vector, C::Y, H::Get(vector, C::Y) * scalar);
        H::Set(vector, C::Z, H::Get(vector, C::Z) * scalar);
    }

    template<typename T> void operator/=(M::Vector4<T>& vector, const T scalar)
    {
        H::Set(vector, C::X, H::Get(vector, C::X) / scalar);
        H::Set(vector, C::Y, H::Get(vector, C::Y) / scalar);
        H::Set(vector, C::Z, H::Get(vector, C::Z) / scalar);
    }

    template<typename T> M::Point4<T>  operator+ (const M::Point4<T>& first, const M::Vector4<T>& second)
    {   
        return Helpers::MakePoint(AddTuples(first, second));
    }

    template<typename T> M::Vector4<T> operator+ (const M::Vector4<T>& first, const M::Vector4<T>& second)
    {
        return Helpers::MakeVector(AddTuples(first, second));
    }

    template<typename T> M::Point4<T>  operator- (const M::Point4<T>& first, const M::Vector4<T>& second)
    {
        return Helpers::MakePoint(SubtractTuples(first, second));
    }

    template<typename T> M::Vector4<T> operator- (const M::Point4<T>& first, const M::Point4<T>& second)
    {
        return Helpers::MakeVector(SubtractTuples(first, second));
    }

    template<typename T> M::Vector4<T> operator- (const M::Vector4<T>& first, const M::Vector4<T>& second)
    {
        return Helpers::MakeVector(SubtractTuples(first, second));
    }
}

namespace Math
{

#pragma region Tuple
    template<typename T> Tuple4<T>::Tuple4(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    template<typename T> Tuple4<T> Tuple4<T>::GetNegated()
    {
        return Tuple4{ x * (T)-1.0, y * (T)-1.0, z * (T)-1.0f, w };
    }

    template<typename T> void Tuple4<T>::Negate()
    {
        this->x *= -1.0;
        this->y *= -1.0;
        this->z *= -1.0;
    }

    template<typename T> Point4<T> Helpers::MakePoint(const T& x, const T& y, const T& z)
    {
        return Point4<T>{ x, y, z, T(1) };
    }

    template<typename T> Vector4<T> Helpers::MakeVector(const T& x, const T& y, const T& z)
    {
        return Vector4<T>{ x, y, z, T(0) };
    }

    template<typename T> Point4<T> Helpers::MakePoint(const Tuple4<T>&& tuple)
    {
        return Point4<T>(tuple);
    }

    template<typename T> Vector4<T> Helpers::MakeVector(const Tuple4<T>&& tuple)
    {
        return Vector4<T>(tuple);
    }

#pragma endregion

#pragma region Point
    template<typename T>
    Math::Point4<T>::Point4(T x, T y, T z) : Tuple4{ x,y,z,T(1) } {}
#pragma endregion

#pragma region Vector
    template<typename T>
    Math::Vector4<T>::Vector4(T x, T y, T z) : Tuple4{ x,y,z, T(0) } {}

    template<typename T> Vector4<T> Vector4<T>::GetNormalized() const
    {
        auto retVal = *this;
        retVal /= GetMagnitude();
        return retVal;
    }

    template<typename T> void Vector4<T>::Normalize()
    {
        *this /= GetMagnitude();
    }

    template<typename T> constexpr T Vector4<T>::GetMagnitudeSquared() const
    {
        return x*x + y*y + z*z;
    }

    template<typename T> constexpr T Vector4<T>::GetMagnitude() const
    {
        return std::sqrt(GetMagnitudeSquared());
    }

    template<typename T> constexpr T Vector4<T>::Dot(const Vector4<T>& other) const
    {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    template<typename T> constexpr Vector4<T> Vector4<T>::Cross(const Vector4<T>& other) const
    {
        return Helpers::MakeVector(
            this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x);
    }
#pragma endregion

}

#pragma region Math Tests
#ifdef _MSC_VER
namespace Math
{
    using H = Math::Helpers;
    using C = Math::Helpers::Coordinate;

    TEST_CLASS(RayTracerTest)
    {
    public:

        TEST_METHOD(AlmostEqualsWorksWithinEpsilon)
        {
            auto epsilon = GetEpsilon<float>();
            auto notOne = 1.0000101f;
            auto one    = 1.000001f;

            Assert::IsFalse(Equalsf(0.0f, 1.0f));
            Assert::IsFalse(Equalsf(notOne, 1.0f));
            Assert::IsTrue(Equalsf(one, 1.0f));
        }

        TEST_METHOD(Vector4StoresTheAppropriateValues)
        {
            auto point = Helpers::MakePoint(1.0f, 2.0f, 3.0f);
            Assert::IsTrue(Equalsf(H::Get(point, C::X), 1.0f));
            Assert::IsTrue(Equalsf(H::Get(point, C::Y), 2.0f));
            Assert::IsTrue(Equalsf(H::Get(point, C::Z), 3.0f));
        }

        TEST_METHOD(Vector4Addition)
        {
            auto point = Helpers::MakePoint(1.3f, -2.2f, 4.1f);
            auto point2 = Helpers::MakePoint(1.3f, -2.2f, 4.1f);

            auto vector = Helpers::MakeVector(2.3f, -5.2f, 3.1f);
            auto vector2 = Helpers::MakeVector(2.3f, -5.2f, 3.1f);

            auto additionResult = point + vector;

            Assert::IsTrue(Equalsf(H::Get(additionResult, C::X), 3.6f));
            Assert::IsTrue(Equalsf(H::Get(additionResult, C::Y), -7.4f));
            Assert::IsTrue(Equalsf(H::Get(additionResult, C::Z), 7.2f));

            auto additionResult2 = vector2 + vector;
            Assert::IsTrue(Equalsf(H::Get(additionResult2, C::X), 4.6f));
            Assert::IsTrue(Equalsf(H::Get(additionResult2, C::Y), -10.4f));
            Assert::IsTrue(Equalsf(H::Get(additionResult2, C::Z), 6.2f));
        }

        TEST_METHOD(Vector4ASubtraction)
        {
            auto point = Helpers::MakePoint(1.3f, -2.2f, 4.1f);
            auto point2 = Helpers::MakePoint(1.3f, -2.2f, 4.1f);
            auto vector = Helpers::MakeVector(2.3f, -5.2f, 3.1f);
            auto vector2 = Helpers::MakeVector(1.5f, 3.3f, 6.3f);


            auto subtractionResults = point - vector;
            Assert::IsTrue(Equalsf(H::Get(subtractionResults, C::X), -1.0f));
            Assert::IsTrue(Equalsf(H::Get(subtractionResults, C::Y), 3.0f));
            Assert::IsTrue(Equalsf(H::Get(subtractionResults, C::Z), 1.0f));

            auto subtractionResults2 = vector2 - vector;
            Assert::IsTrue(Equalsf(H::Get(subtractionResults2, C::X), -0.8f));
            Assert::IsTrue(Equalsf(H::Get(subtractionResults2, C::Y), 8.5f));
            Assert::IsTrue(Equalsf(H::Get(subtractionResults2, C::Z), 3.2f));
            
            auto subtractionResults3 = point2 - point;
            Assert::IsTrue(Equalsf(H::Get(subtractionResults3, C::X), 0.0f));
            Assert::IsTrue(Equalsf(H::Get(subtractionResults3, C::Y), 0.0f));
            Assert::IsTrue(Equalsf(H::Get(subtractionResults3, C::Z), 0.0f));
        }

        TEST_METHOD(Vector4ScalarMultiplicationWorksOnVectors)
        {
            auto vector = Helpers::MakeVector(0.3f, 5.043f, 83.f);
            const auto vectorAfterMultiplication = vector * 2.0f;

            vector *= 2.0f;

            Assert::IsTrue(Equalsf(H::Get(vector, C::X), H::Get(vectorAfterMultiplication, C::X)));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Y), H::Get(vectorAfterMultiplication, C::Y)));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Z), H::Get(vectorAfterMultiplication, C::Z)));

            Assert::IsTrue(Equalsf(H::Get(vector, C::X), 0.6f));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Y), 10.086f));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Z), 166.f));
        }

        TEST_METHOD(Vector4NegationOrInversion)
        {
            auto vector = Helpers::MakeVector(1.3f, -2.2f, 4.1f);
            auto vectorNegated = vector.GetNegated();
            vector.Negate();

            Assert::IsTrue(Equalsf(H::Get(vectorNegated, C::X), -1.3f));
            Assert::IsTrue(Equalsf(H::Get(vectorNegated, C::Y), 2.2f));
            Assert::IsTrue(Equalsf(H::Get(vectorNegated, C::Z), -4.1f));

            Assert::IsTrue(Equalsf(H::Get(vectorNegated, C::X), H::Get(vector, C::X)));
            Assert::IsTrue(Equalsf(H::Get(vectorNegated, C::Y), H::Get(vector, C::Y)));
            Assert::IsTrue(Equalsf(H::Get(vectorNegated, C::Z), H::Get(vector, C::Z)));
        }

        TEST_METHOD(Vector4ScalarDivisionWorksOnVectors)
        {
            auto vector = Helpers::MakeVector(0.3f, 5.043f, 83.f);
            const auto vectorAfterDivision = vector / 2.0f;

            vector /= 2.0f;

            Assert::IsTrue(Equalsf(H::Get(vector, C::X), H::Get(vectorAfterDivision, C::X)));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Y), H::Get(vectorAfterDivision, C::Y)));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Z), H::Get(vectorAfterDivision, C::Z)));

            Assert::IsTrue(Equalsf(H::Get(vector, C::X), 0.15f));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Y), 2.5215f));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Z), 41.5f));
        }

        TEST_METHOD(Vector4Magnitude)
        {
            const auto vector = Helpers::MakeVector(0.3f, 5.043f, 83.f);
            const auto point = Helpers::MakePoint(0.3f, 5.043f, 83.f);

            float magnitudeSquared = vector.GetMagnitudeSquared();//6914.521849
            Assert::IsTrue(Equalsf(magnitudeSquared, 6914.5218f));

            float magnitude = vector.GetMagnitude();
            Assert::IsTrue(Equalsf(magnitude, 83.1536f)); //83.15360394474794
        }

        TEST_METHOD(Vector4Normalization)
        {
            auto vector = Helpers::MakeVector(0.3f, 5.043f, 83.f);
            const auto point = Helpers::MakePoint(0.3f, 5.043f, 83.f);

            const auto vectorNormalized = vector.GetNormalized();
            vector.Normalize();

            Assert::IsTrue(Equalsf(H::Get(vector, C::X), 0.003607f));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Y), 0.060646f));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Z), 0.998153f));

            Assert::IsTrue(Equalsf(H::Get(vector, C::X), H::Get(vectorNormalized, C::X)));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Y), H::Get(vectorNormalized, C::Y)));
            Assert::IsTrue(Equalsf(H::Get(vector, C::Z), H::Get(vectorNormalized, C::Z)));
        }

        TEST_METHOD(Vector4Dot)
        {
            const auto vector1 = Helpers::MakeVector(1.0f, 2.0f, 3.0f);
            const auto vector2 = Helpers::MakeVector(2.0f, 3.0f, 4.0f);

            auto dotResult = vector1.Dot(vector2);
            Assert::AreEqual(dotResult, 20.0f);
        }

        TEST_METHOD(Vector4Cross)
        {
            const auto vector1 = Helpers::MakeVector(1.0f, 2.0f, 3.0f);
            const auto vector2 = Helpers::MakeVector(2.0f, 3.0f, 4.0f);

            const auto cross12 = vector1.Cross(vector2);
            const auto cross21 = vector2.Cross(vector1);

            const auto expected12 = Helpers::MakeVector(-1.0f, 2.0f, -1.0f);
            const auto expected21 = Helpers::MakeVector(1.0f, -2.0f, 1.0f);

            Assert::IsTrue(cross12 == expected12);
            Assert::IsTrue(cross21 == expected21);
        }
    };
}
#endif
#pragma endregion