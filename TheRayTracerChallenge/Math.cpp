#include "stdafx.h"
#include "Math.h"

#ifdef _MSC_VER
    #include "CppUnitTest.h"
    using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif


namespace M = Math;
using H = Math::Helpers;
using C = Math::Helpers::Coordinate;
using CI = Math::Helpers::ColorInput;

namespace Math
{
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

	template<typename T>
	constexpr void MultiplyTupleByScalar(M::Tuple4<T>& tuple, const T scalar)
	{
		H::Set(tuple, C::X, H::Get(tuple, C::X) * scalar);
		H::Set(tuple, C::Y, H::Get(tuple, C::Y) * scalar);
		H::Set(tuple, C::Z, H::Get(tuple, C::Z) * scalar);
	}

	template<typename T>
	constexpr void DivideTupleByScalar(M::Tuple4<T>& tuple, const T scalar)
	{
		H::Set(tuple, C::X, H::Get(tuple, C::X) / scalar);
		H::Set(tuple, C::Y, H::Get(tuple, C::Y) / scalar);
		H::Set(tuple, C::Z, H::Get(tuple, C::Z) / scalar);
	}

	template<typename T>
	constexpr M::Tuple4<T> MultiplyTupleByMatrix(M::Tuple4<T>& tuple, M::SquareMatrix<T, 4>& matrix)
	{
		M::Tuple4<T> retVal{0.0f, 0.0f, 0.0f, 0.0f};

		static const size_t Size = 4;


		for (size_t i = 0; i < Size; ++i)
		{
			for (size_t j = 0; j < Size; ++j)
			{
				H::Set(retVal, C(i),
					H::Get(retVal, C(i)) + 
					matrix.GetValueAt(i, j) * H::Get(tuple, C(j)));
			}
		}

		return retVal;
	}

	template<typename T> M::Vector4<T> operator*(const M::Vector4<T>& vector, const T scalar)
	{
		M::Vector4<T> retVal = vector;
		MultiplyTupleByScalar(retVal, scalar);
		return retVal;
	}

	template<typename T> M::Color4<T> operator*(const M::Color4<T>& color, const T scalar)
	{
		M::Color4<T> retVal = color;
		MultiplyTupleByScalar(retVal, scalar);
		return retVal;
	}

	template<typename T> Math::Color4<T> operator*(const Math::Color4<T>& color1, const Math::Color4<T>& color2)
	{
		Color4<T> retVal = color1;
		retVal.Hadamard(color2);
		return retVal;
	}

	template<typename T, size_t Size> 
	Math::SquareMatrix<T, Size> operator*(
		Math::SquareMatrix<T, Size>& matrix1,
		Math::SquareMatrix<T, Size>& matrix2)
	{
		Math::SquareMatrix<T, Size> retVal;
		
		for (size_t lineIndex = 0; lineIndex < Size; ++lineIndex)
		{
			for (size_t columnIndex = 0; columnIndex < Size; ++columnIndex)
			{
				T valueToSet = retVal.GetValueAt(lineIndex, columnIndex);
				retVal.SetValueAt(lineIndex, columnIndex, 0);
				valueToSet = 0;

				for (size_t indexMul = 0; indexMul < Size; ++indexMul)
				{
					valueToSet +=
						matrix1.GetValueAt(lineIndex, indexMul) *
						matrix2.GetValueAt(indexMul, columnIndex);
				}

				retVal.SetValueAt(lineIndex, columnIndex, valueToSet);
			}
		}

		return retVal;
	}


	template<typename T>
	Math::Vector4<T> operator*(
		const Math::Vector4<T>& vector, 
		Math::SquareMatrix<T, 4>& matrix)
	{
		M::Tuple4<T> retValT = MultiplyTupleByMatrix(static_cast<M::Tuple4<T>>(vector), matrix);		
		return  H::MakeVector(retValT);
	}

	template<typename T> M::Vector4<T> operator/(const M::Vector4<T>& vector, const T scalar)
	{
		M::Vector4<T> retVal = vector;
		DivideTupleByScalar(retVal, scalar);
		return retVal;
	}

	template<typename T> M::Color4<T> operator/(const M::Color4<T>& color, const T scalar)
	{
		M::Color4<T> retVal = color;
		DivideTupleByScalar(retVal, scalar);
		return retVal;
	}

	template<typename T> void operator*=(M::Vector4<T>& vector, const T scalar)
	{
		MultiplyTupleByScalar(vector, scalar);
	}

	template<typename T> void operator*=(M::Color4<T>& color, const T scalar)
	{
		MultiplyTupleByScalar(color, scalar);
	}

	template<typename T> void operator*= (Math::Color4<T>& color, const Math::Color4<T>& colorOther)
	{
		color.Hadamard(colorOther);
	}

    template<typename T> void operator/=(M::Vector4<T>& vector, const T scalar)
    {
		DivideTupleByScalar(vector, scalar);
    }

	template<typename T> void operator/=(M::Color4<T>& color, const T scalar)
	{
		DivideTupleByScalar(color, scalar);
	}

    template<typename T> M::Point4<T>  operator+ (const M::Point4<T>& first, const M::Vector4<T>& second)
    {   
        return H::MakePoint(AddTuples(first, second));
    }

    template<typename T> M::Vector4<T> operator+ (const M::Vector4<T>& first, const M::Vector4<T>& second)
    {
        return H::MakeVector(AddTuples(first, second));
    }

	template<typename T> Math::Color4<T> operator+ (const Math::Color4<T>& first, const Math::Color4<T>& second)
	{
		return H::MakeColor(AddTuples(first, second));
	}

    template<typename T> M::Point4<T>  operator- (const M::Point4<T>& first, const M::Vector4<T>& second)
    {
        return H::MakePoint(SubtractTuples(first, second));
    }

    template<typename T> M::Vector4<T> operator- (const M::Point4<T>& first, const M::Point4<T>& second)
    {
        return H::MakeVector(SubtractTuples(first, second));
    }

    template<typename T> M::Vector4<T> operator- (const M::Vector4<T>& first, const M::Vector4<T>& second)
    {
        return H::MakeVector(SubtractTuples(first, second));
    }

	template<typename T> Math::Color4<T> operator- (const Math::Color4<T>& first, const Math::Color4<T>& second)
	{
		return H::MakeColor(SubtractTuples(first, second));
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

	template<typename T> Color4<T> Helpers::MakeColor(const T& r, const T& g, const T& b, const T& a)
	{
		return Color4<T>{ r, g, b, a };
	}

    template<typename T> Point4<T> Helpers::MakePoint(const Tuple4<T>& tuple)
    {
        return Point4<T>(tuple);
    }

    template<typename T> Vector4<T> Helpers::MakeVector(const Tuple4<T>& tuple)
    {
        return Vector4<T>(tuple);
    }

	template<typename T> Color4<T> Helpers::MakeColor(const Tuple4<T>& tuple)
	{
		return Color4<T>(tuple);
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

#pragma region Color

	template<typename T>
	Math::Color4<T>::Color4(T r, T g, T b, T a) : Tuple4{r, g, b, a} {};

	template<typename T> constexpr void Math::Color4<T>::Hadamard(const Color4<T>& other)
	{
		this->x *= H::Get(other, CI::R);
		this->y *= H::Get(other, CI::G);
		this->z *= H::Get(other, CI::B);
		this->w *= H::Get(other, CI::A);
	}


#pragma endregion

}

#pragma region Math Tests
#ifdef _MSC_VER
namespace Math
{
    TEST_CLASS(MathTest)
    {

    public:
		
		TEST_METHOD(Assert)
		{
			bool exceptionCaught = false;

			try
			{
				ASSERT(false);
			}

			catch (...)
			{
				exceptionCaught = true;
			}

			Assert::IsTrue(exceptionCaught);			
		}

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
            auto point = H::MakePoint(1.3f, -2.2f, 4.1f);
            auto point2 = H::MakePoint(1.3f, -2.2f, 4.1f);

            auto vector = H::MakeVector(2.3f, -5.2f, 3.1f);
            auto vector2 = H::MakeVector(2.3f, -5.2f, 3.1f);

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
            auto point = H::MakePoint(1.3f, -2.2f, 4.1f);
            auto point2 = H::MakePoint(1.3f, -2.2f, 4.1f);
            auto vector = H::MakeVector(2.3f, -5.2f, 3.1f);
            auto vector2 = H::MakeVector(1.5f, 3.3f, 6.3f);


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
            auto vector = H::MakeVector(0.3f, 5.043f, 83.f);
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
            auto vector = H::MakeVector(1.3f, -2.2f, 4.1f);
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
            auto vector = H::MakeVector(0.3f, 5.043f, 83.f);
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
            const auto vector = H::MakeVector(0.3f, 5.043f, 83.f);
            const auto point = H::MakePoint(0.3f, 5.043f, 83.f);

            float magnitudeSquared = vector.GetMagnitudeSquared();//6914.521849
            Assert::IsTrue(Equalsf(magnitudeSquared, 6914.5218f));

            float magnitude = vector.GetMagnitude();
            Assert::IsTrue(Equalsf(magnitude, 83.1536f)); //83.15360394474794
        }

        TEST_METHOD(Vector4Normalization)
        {
            auto vector = H::MakeVector(0.3f, 5.043f, 83.f);
            const auto point = H::MakePoint(0.3f, 5.043f, 83.f);

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
            const auto vector1 = H::MakeVector(1.0f, 2.0f, 3.0f);
            const auto vector2 = H::MakeVector(2.0f, 3.0f, 4.0f);

            auto dotResult = vector1.Dot(vector2);
            Assert::AreEqual(dotResult, 20.0f);
        }

        TEST_METHOD(Vector4Cross)
        {
            const auto vector1 = H::MakeVector(1.0f, 2.0f, 3.0f);
            const auto vector2 = H::MakeVector(2.0f, 3.0f, 4.0f);

            const auto cross12 = vector1.Cross(vector2);
            const auto cross21 = vector2.Cross(vector1);

            const auto expected12 = H::MakeVector(-1.0f, 2.0f, -1.0f);
            const auto expected21 = H::MakeVector(1.0f, -2.0f, 1.0f);

            Assert::IsTrue(cross12 == expected12);
            Assert::IsTrue(cross21 == expected21);
        }

		TEST_METHOD(Color4Values)
		{
			const auto color1 = H::MakeColor(-0.5f, 0.4f, 1.7f, 0.5f);
			
			Assert::IsTrue(Equalsf(H::Get(color1, CI::R), -0.5f));
			Assert::IsTrue(Equalsf(H::Get(color1, CI::G), 0.4f));
			Assert::IsTrue(Equalsf(H::Get(color1, CI::B), 1.7f));
			Assert::IsTrue(Equalsf(H::Get(color1, CI::A), 0.5f));
		}

		TEST_METHOD(Color4Addition)
		{
			const auto color1 = H::MakeColor(-0.5f, 0.4f, 1.7f, 0.5f);
			const auto color2 = H::MakeColor(-0.4f, 0.3f, 1.6f, 0.4f);

			const auto colorAdded = color1 + color2;

			Assert::IsTrue(Equalsf(H::Get(colorAdded, CI::R), -0.9f));
			Assert::IsTrue(Equalsf(H::Get(colorAdded, CI::G), 0.7f));
			Assert::IsTrue(Equalsf(H::Get(colorAdded, CI::B), 3.3f));
			Assert::IsTrue(Equalsf(H::Get(colorAdded, CI::A), 0.9f));
		}

		TEST_METHOD(Color4Subtraction)
		{
			const auto color1 = H::MakeColor(-0.5f, 0.4f, 1.7f, 0.5f);
			const auto color2 = H::MakeColor(-0.4f, 0.3f, 1.6f, 0.4f);

			const auto colorSubtracted = color1 - color2;

			Assert::IsTrue(Equalsf(H::Get(colorSubtracted, CI::R), -0.1f));
			Assert::IsTrue(Equalsf(H::Get(colorSubtracted, CI::G), 0.1f));
			Assert::IsTrue(Equalsf(H::Get(colorSubtracted, CI::B), 0.1f));
			Assert::IsTrue(Equalsf(H::Get(colorSubtracted, CI::A), 0.1f));
		}

		TEST_METHOD(Color4ScalarMultiplication)
		{
			const auto color1 = H::MakeColor(-0.5f, 0.4f, 1.7f, 0.5f);
			const auto multiplier = 2.0f;

			const auto colorMultiplied = color1 * multiplier;

			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::R), -1.0f));
			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::G), 0.8f));
			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::B), 3.4f));
			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::A), 0.5f));
		}

		TEST_METHOD(Color4ScalarDivision)
		{
			const auto color1 = H::MakeColor(-0.6f, 0.4f, 2.0f, 0.6f);
			const auto divider = 2.0f;

			const auto colorDivided = color1 / divider;

			Assert::IsTrue(Equalsf(H::Get(colorDivided, CI::R), -0.3f));
			Assert::IsTrue(Equalsf(H::Get(colorDivided, CI::G), 0.2f));
			Assert::IsTrue(Equalsf(H::Get(colorDivided, CI::B), 1.0f));
			Assert::IsTrue(Equalsf(H::Get(colorDivided, CI::A), 0.6f));
		}

		TEST_METHOD(Color4Color4Multiplication)
		{
				  auto color1 = H::MakeColor(-0.5f, 0.4f, 1.7f, 0.5f);
			const auto color2 = H::MakeColor(-0.4f, 0.3f, 1.6f, 0.4f);

			const auto colorMultiplied = color1 * color2;
			color1 *= color2;

			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::R), 0.2f));
			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::G), 0.12f));
			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::B), 2.72f));
			Assert::IsTrue(Equalsf(H::Get(colorMultiplied, CI::A), 0.2f));

			Assert::IsTrue(Equalsf(H::Get(color1, CI::R), 0.2f));
			Assert::IsTrue(Equalsf(H::Get(color1, CI::G), 0.12f));
			Assert::IsTrue(Equalsf(H::Get(color1, CI::B), 2.72f));
			Assert::IsTrue(Equalsf(H::Get(color1, CI::A), 0.2f));
		}

		TEST_METHOD(MatrixCreation)
		{
			auto matrix1 = SquareMatrix<float, 2>({ 1.0f, 2.0f, 3.0f, 4.0f });

			Assert::IsTrue(Equalsf(matrix1.GetValueAt(0, 0), 1.0f));
			Assert::IsTrue(Equalsf(matrix1.GetValueAt(0, 1), 2.0f));
			Assert::IsTrue(Equalsf(matrix1.GetValueAt(1, 0), 3.0f));
			Assert::IsTrue(Equalsf(matrix1.GetValueAt(1, 1), 4.0f));
		}

		TEST_METHOD(MatrixGetLine)
		{
			auto matrix = SquareMatrix<float, 3>({
				1.0f, 2.0f, 4.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto line = matrix.GetLineAt(0);
			auto expected = std::array<float, 3>{1.0f, 2.0f, 4.0f};

			Assert::IsTrue(line == expected);

			matrix.SetTransposed(true);
			line = matrix.GetLineAt(0);
			expected = std::array<float, 3>{1.0f, 4.0f, 7.0f};
			Assert::IsTrue(line == expected);
		}

		TEST_METHOD(MatrixGetColumn)
		{
			auto matrix = SquareMatrix<float, 3>({
				1.0f, 2.0f, 4.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto line = matrix.GetColumnAt(0);
			auto expected = std::array<float, 3>{1.0f, 4.0f, 7.0f};
			
			Assert::IsTrue(line == expected);

			matrix.SetTransposed(true);
			line = matrix.GetColumnAt(0);
			expected = std::array<float, 3>{1.0f, 2.0f, 4.0f};
			Assert::IsTrue(line == expected);
		}


		TEST_METHOD(MatrixIdentity)
		{
			auto matrix = SquareMatrix<float, 4>::Identity();

			auto expectedResult = SquareMatrix<float, 4>({
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f });

			Assert::IsTrue(expectedResult == matrix);
		}

		TEST_METHOD(MatrixEquality)
		{
			auto matrix1 = SquareMatrix<float, 3>({ 
				1.0f, 2.0f, 3.0f, 
				4.0f, 5.0f, 6.0f, 
				7.0f, 8.0f, 9.0f });
			
			auto matrix2 = SquareMatrix<float, 3>({ 
				1.0f, 2.0f, 3.0f, 
				4.0f, 5.0f, 6.0f, 
				7.0f, 8.0f, 9.0f });
			
			auto matrix3 = SquareMatrix<float, 3>({ 
				1.0f, 2.0f, 4.0f, 
				4.0f, 5.0f, 6.0f, 
				7.0f, 8.0f, 9.0f });

			Assert::IsTrue(matrix1 == matrix2);
			Assert::IsFalse(matrix1 == matrix3);
		}

		TEST_METHOD(MatrixMultiplication)
		{
			auto matrix1 = SquareMatrix<float, 3>({ 
				1.0f, 2.0f, 3.0f, 
				4.0f, 5.0f, 6.0f, 
				7.0f, 8.0f, 9.0f });

			auto matrix2 = SquareMatrix<float, 3>({
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f });

			auto matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrix1);
			
			matrix2 = SquareMatrix<float, 3>({
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f });

			auto matrixExpectation = SquareMatrix<float, 3>({ 
				6.0f, 6.0f, 6.0f, 
				15.0f, 15.0f, 15.0f, 
				24.0f, 24.0f, 24.0f });

			matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrixExpectation);
		}

		TEST_METHOD(MatrixMultiplicationTransposed)
		{
			auto matrix1 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			matrix1.SetTransposed(true);

			auto matrix2 = SquareMatrix<float, 3>({
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f });

			auto matrixResult = matrix1 * matrix2;
			auto matrixExpectation = SquareMatrix<float, 3>({
				1.0f, 4.0f, 7.0f,
				2.0f, 5.0f, 8.0f,
				3.0f, 6.0f, 9.0f });

			Assert::IsTrue(matrixResult == matrixExpectation);

			matrix2 = SquareMatrix<float, 3>({
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f });

			matrixExpectation = SquareMatrix<float, 3>({
				12.0f, 12.0f, 12.0f,
				15.0f, 15.0f, 15.0f,
				18.0f, 18.0f, 18.0f });

			matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrixExpectation);
		}

		TEST_METHOD(VectorByMatrixMultiplication)
		{
			auto vector = H::MakeVector<float>(1.0f, 2.0f, 3.0f);
			auto matrix = SquareMatrix<float, 4>::Identity();

			auto multiplicationResult = vector * matrix;
			auto expectedResult = vector;

			Assert::IsTrue(expectedResult == multiplicationResult);

			matrix = SquareMatrix<float, 4>({
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f });


			expectedResult = H::MakeVector<float>(14.0f, 38.0f, 62.0f);
			multiplicationResult = vector * matrix;

			Assert::IsTrue(expectedResult == multiplicationResult);
		}

		TEST_METHOD(VectorByMatrixMultiplicationTransposed)
		{
			auto vector = H::MakeVector<float>(1.0f, 2.0f, 3.0f);
			auto matrix = SquareMatrix<float, 4>::Identity();
			matrix.SetTransposed(true);

			auto multiplicationResult = vector * matrix;
			auto expectedResult = vector;

			Assert::IsTrue(expectedResult == multiplicationResult);

			matrix = SquareMatrix<float, 4>({
				1.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				13.0f, 14.0f, 15.0f, 16.0f });
			
			matrix.SetTransposed(true);

			expectedResult = H::MakeVector<float>(38.0f, 44.0f, 50.0f);
			multiplicationResult = vector * matrix;

			Assert::IsTrue(expectedResult == multiplicationResult);
		}

		TEST_METHOD(MatrixDeterminant_Helpers_Order2)
		{
			auto matrix = SquareMatrix<float, 2>({
				1.0f, 2.0f,
				4.0f, 5.0f });

			Assert::IsTrue(Equalsf(matrix.GetDeterminant(), -3.0f));
		}

		TEST_METHOD(MatrixDeterminant_Helpers_Order3)
		{
			auto matrix = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				2.0f, 4.0f, 1.0f});

			Assert::IsTrue(Equalsf(matrix.GetDeterminant(), 15.0f));
		}

		TEST_METHOD(MatrixDeterminant_Helpers_Order4)
		{
			auto matrix = SquareMatrix<float, 4>({
				11.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				4.0f, 3.0f, 2.0f, 15.0f });

			auto det = matrix.GetDeterminant();
			Assert::IsTrue(Equalsf(det, -560.0f));
			
			matrix = SquareMatrix<float, 4>({
				6.0f, 4.0f, 4.0f, 4.0f,
				5.0f, 5.0f, 7.0f, 6.0f,
				4.0f, -9.0f, 3.0f, -7.0f,
				9.0f, 1.0f, 7.0f, -6.0f	});

			det = matrix.GetDeterminant();
			Assert::IsTrue(Equalsf(det, -2120.0f));

			matrix = SquareMatrix<float, 4>({
				-4.0f, 2.0f, -2.0f, -3.0f,
				9.0f, 6.0f, 2.0f, 6.0f,
				0.0f, -5.0f, 1.0f, -5.0f,
				0.0f, 0.0f, 0.0f, 0.0f });

			det = matrix.GetDeterminant();
			Assert::IsTrue(Equalsf(det, 0.0f));
		}
		TEST_METHOD(MatrixDeterminant_Helpers_OrderHigherThan4)
		{			
			auto matrix = SquareMatrix<float, 5>({
				11.0f,	2.0f,	3.0f,	4.0f,	5.0f,
				5.0f,	6.0f,	7.0f,	8.0f,	12.0f,
				9.0f,	10.0f,	11.0f,	12.0f,	2.0f,
				4.0f,	3.0f,	2.0f,	1.0f,	3.0f,
				21.0f,	15.0f,	1.0f,	7.0f,	10.0f});

			Assert::IsTrue(Equalsf(matrix.GetDeterminant(), 30000.0f));		
		}

		TEST_METHOD(Matrix4_Submatrices)
		{
			auto matrix = SquareMatrix<float, 4>({
				11.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				4.0f, 3.0f, 2.0f, 15.0f });

			auto subMatrices = GetSubmatrices(matrix);
			
			auto expectation = SquareMatrix<float, 3>({
				6.0f, 7.0f, 8.0f,
				10.0f, 11.0f, 12.0f,
				3.0f, 2.0f, 15.0f });			
			Assert::IsTrue(subMatrices[0] == expectation);

			expectation = SquareMatrix<float, 3>({
				5.0f, 7.0f, 8.0f,
				9.0f, 11.0f, 12.0f,
				4.0f, 2.0f, 15.0f });
			Assert::IsTrue(subMatrices[1] == expectation);

			expectation = SquareMatrix<float, 3>({
				5.0f, 6.0f, 8.0f,
				9.0f, 10.0f, 12.0f,
				4.0f, 3.0f, 15.0f });
			Assert::IsTrue(subMatrices[2] == expectation);

			expectation = SquareMatrix<float, 3>({
				5.0f, 6.0f, 7.0f,
				9.0f, 10.0f, 11.0f,
				4.0f, 3.0f, 2.0f });
			Assert::IsTrue(subMatrices[3] == expectation);

		}

		TEST_METHOD(Inverse_Matrix4)
		{	
			//minor = determinant of a submatrix
			//cofactor = determinant of a submatrix * (-1)^minorPosition
			auto expectation = SquareMatrix<float, 4>({
				0.21805f, 0.45113f, 0.24060f, -0.04511f
				-0.80827f, -1.45677f, -0.44361f, 0.52068f,
				-0.07895f, -0.22368f, -0.05263f, 0.19737f,
				-0.52256f, -0.81391f, -0.30075f, 0.30639f });

			auto matrix = SquareMatrix<float, 4>({
				-5.0f, 2.0f, 6.0f, -8.0f,
				1.0f, -5.0f, 1.0f, 8.0f,
				7.0f, 7.0f, -6.0f, -7.0f,
				1.0f, -3.0f, 7.0f, 4.0f });

			auto matrixInverse = matrix.GetInverse();
			Assert::IsTrue(matrixInverse == expectation);
		}

    };
}

#endif
#pragma endregion