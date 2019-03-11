#include "stdafx.h"

#include "Math_Tuple.h"
#include "Math_Transform.h"

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

using H = Math::Helpers;
using C = Math::Helpers::Coordinate;
using CI = Math::Helpers::ColorInput;

namespace Math
{

	template<typename T>
	SquareMatrix<T, 4> Transform<T>::MakeTranslation(const T x, const T y, const T z)
	{
		SquareMatrix<T, 4> translation = SquareMatrix<T, 4>::Identity();
		translation.SetOriginalValueAt(0, 3, x);
		translation.SetOriginalValueAt(1, 3, y);
		translation.SetOriginalValueAt(2, 3, z);

		return translation;
	}

	template<typename T>
	SquareMatrix<T, 4> Transform<T>::MakeScaling(const T x, const T y, const T z)
	{
		SquareMatrix<T, 4> scaling;
		return scaling;
	}

	template<typename T>
	SquareMatrix<T, 4> Transform<T>::MakeRotation(const T angleX, T angleY, T angleZ)
	{
		SquareMatrix<T, 4> rotation;
		return rotation;
	}


	template<typename T>
	SquareMatrix<T, 4> Transform<T>::GetTranslation()
	{
		SquareMatrix<T, 4> translation = SquareMatrix<T, 4>::Identity();		
		return translation;
	}

	template<typename T>
	SquareMatrix<T, 4> Transform<T>::GetScaling()
	{
		SquareMatrix<T, 4> scaling;

		return scaling;
	}

	template<typename T>
	SquareMatrix<T, 4> Transform<T>::GetRotation()
	{
		SquareMatrix<T, 4> rotation;
		for (size_t i = 0; i < 3; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				rotation.SetOriginalValueAt(i, j, this->GetValueAt(i, j));
			}
		}

		return rotation;
	}
}

#pragma region Math Tests
#ifdef _MSC_VER
namespace Math
{
	TEST_CLASS(TestMathTransform)
	{
	public:
		TEST_METHOD(Transform_VectorTranslation)
		{
			auto point = H::MakePoint<float>(-3.0f, 4.0f, 5.0f);
			auto vector = H::MakeVector<float>(-3.0f, 4.0f, 5.0f);
			auto translation = Transform<float>::MakeTranslation(5.0f, -3.0f, 2.0f);

			auto translatedVector = vector * translation;
			auto translatedPoint = point * translation;

			Assert::IsTrue(Equalsf(H::Get(translatedPoint, C::X), 2.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedPoint, C::Y), 1.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedPoint, C::Z), 7.0f));

			Assert::IsTrue(Equalsf(H::Get(translatedVector, C::X), -3.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedVector, C::Y), 4.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedVector, C::Z), 5.0f));
		}	
	};
}
#endif