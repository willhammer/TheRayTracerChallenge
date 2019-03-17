#include "stdafx.h"

#include "Math_Tuple.h"
#include "Math_Transform.h"
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
	auto MakeRotationX(const T radiansX) ->
		std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>
	{
		Math::SquareMatrix<T, 4> rotation = Math::SquareMatrix<T, 4>::Identity();
		T cosineX = std::cos(radiansX);
		T sineX = std::sin(radiansX);

		//for rotation around axis X
		rotation.SetOriginalValueAt(1, 1, cosineX);
		rotation.SetOriginalValueAt(1, 2, -sineX);
		rotation.SetOriginalValueAt(2, 1, sineX);
		rotation.SetOriginalValueAt(2, 2, cosineX);

		return rotation;
	}

	template<typename T>
	auto MakeRotationY(const T radiansY) ->
		std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>
	{
		Math::SquareMatrix<T, 4> rotation = Math::SquareMatrix<T, 4>::Identity();
		T cosineY = std::cos(radiansY);
		T sineY = std::sin(radiansY);

		//for rotation around axis Y
		rotation.SetOriginalValueAt(0, 0, cosineY);
		rotation.SetOriginalValueAt(0, 2, sineY);
		rotation.SetOriginalValueAt(2, 0, -sineY);
		rotation.SetOriginalValueAt(2, 2, cosineY);

		return rotation;
	}

	template<typename T>
	auto MakeRotationZ(const T radiansZ) ->
		std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>
	{
		Math::SquareMatrix<T, 4> rotation = Math::SquareMatrix<T, 4>::Identity();
		T cosineZ = std::cos(radiansZ);
		T sineZ = std::sin(radiansZ);

		//for rotation around axis Z				
		rotation.SetOriginalValueAt(0, 0, cosineZ);
		rotation.SetOriginalValueAt(0, 1, -sineZ);
		rotation.SetOriginalValueAt(1, 0, sineZ);
		rotation.SetOriginalValueAt(1, 1, cosineZ);

		return rotation;
	}


	template<typename T>
	auto MakeRotationInplace(const T radiansX, const T radiansY, const T radiansZ) ->
		std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>
	{		
		T cosineX = std::cos(radiansX);
		T sineX = std::sin(radiansX);

		T cosineY = std::cos(radiansY);
		T sineY = std::sin(radiansY);

		T cosineZ = std::cos(radiansZ);
		T sineZ = std::sin(radiansZ);

		Math::SquareMatrix<T, 4> rotation({
			(cosineY * cosineZ),							(-cosineY * sineZ ),							(sineY),			0.0f,
			(sineX * sineY * cosineZ + cosineX * sineZ),	(-sineX * sineY * sineZ + cosineX * cosineZ),	(-sineX * cosineY),	0.0f,
			(-cosineX * sineY * cosineZ + sineX * sineZ),	(cosineX * sineY * sineZ + sineX * cosineZ), 	(cosineX * cosineY),0.0f,
			0.0f,											0.0f,											0.0f,				1.0f});

		return rotation;
	}
}


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
		SquareMatrix<T, 4> scaling = SquareMatrix<T, 4>::Identity();
		scaling.SetOriginalValueAt(0, 0, x);
		scaling.SetOriginalValueAt(1, 1, y);
		scaling.SetOriginalValueAt(2, 2, z);
		return scaling;
	}

	template<typename T>
	auto Transform<T>::MakeRotationEuler(const T radiansX, const T radiansY, const T radiansZ) ->
		std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>
	{
		SquareMatrix<T, 4> rotation = 
			MakeRotationX<T>(radiansX) * MakeRotationY<T>(radiansY) * MakeRotationZ<T>(radiansZ);

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
		SquareMatrix<T, 4> scaling = SquareMatrix<T, 4>::Zero();

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

		TEST_METHOD(Transform_VectorScaling)
		{
			auto point = H::MakePoint<float>(2.0f, 3.0f, 4.0f);
			auto vector = H::MakeVector<float>(2.0f, 3.0f, 4.0f);
			auto scaling = Transform<float>::MakeScaling(-1.0f, 3.0f, -2.0f);

			auto translatedVector = vector * scaling;
			auto translatedPoint = point * scaling;

			Assert::IsTrue(Equalsf(H::Get(translatedPoint, C::X), -2.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedPoint, C::Y), 9.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedPoint, C::Z), -8.0f));

			Assert::IsTrue(Equalsf(H::Get(translatedVector, C::X), -2.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedVector, C::Y), 9.0f));
			Assert::IsTrue(Equalsf(H::Get(translatedVector, C::Z), -8.0f));
		}

		TEST_METHOD(Transform_VectorRotation)
		{
			auto pointX = H::MakePoint<float>(0.0f, 1.0f, 0.0f);
			auto pointY = H::MakePoint<float>(0.0f, 0.0f, 1.0f);
			auto pointZ = H::MakePoint<float>(0.0f, 1.0f, 0.0f);

			auto rotation = Transform<float>::MakeRotationEuler(GetPiBy2<float>(), 0.0f, 0.0f);
			auto rotationInplace = MakeRotationInplace(GetPiBy2<float>(), 0.0f, 0.0f);
			Assert::IsTrue(rotation == rotationInplace);

			auto pointRotated = pointX * rotation;
			auto pointRotatedExpectation = H::MakePoint<float>(0.0f, 0.0f, 1.0f);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);
		}
	};
}
#endif