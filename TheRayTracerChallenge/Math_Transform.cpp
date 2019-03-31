#include "stdafx.h"
#include "Math_Transform.h"
#include "Math_Tuple.h"
#include "Math_Matrix.h"

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
	Math::Transform<T> MakeRotationX(const T radiansX)
	{
		Math::Transform<T> rotation = Math::Transform<T>::Identity();
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
	Math::Transform<T> MakeRotationY(const T radiansY)
	{
		Math::Transform<T> rotation = Math::Transform<T>::Identity();
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
	Math::Transform<T> MakeRotationZ(const T radiansZ)
	{
		Math::Transform<T> rotation = Math::Transform<T>::Identity();
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
	Math::Transform<T> MakeRotationCompound(const T radiansX, const T radiansY, const T radiansZ)
	{
		return MakeRotationX<T>(radiansX) * MakeRotationY<T>(radiansY) * MakeRotationZ<T>(radiansZ);
	}

	template<typename T>
	Math::Transform<T> MakeRotationInplace(const T radiansX, const T radiansY, const T radiansZ)
	{		
		T cosineX = std::cos(radiansX);
		T sineX = std::sin(radiansX);

		T cosineY = std::cos(radiansY);
		T sineY = std::sin(radiansY);

		T cosineZ = std::cos(radiansZ);
		T sineZ = std::sin(radiansZ);

		Math::Transform<T> rotation({
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
	Transform<T> operator*(Transform<T>& transform1, Transform<T>& transform2)
	{
		return GetMultipliedContents(transform1, transform2);
	}

	template<typename T>
	Transform<T> operator+(Transform<T>& transform1, Transform<T>& transform2)
	{
		return GetAddedContents(transform1, transform2);
	}

	template<typename T>
	bool operator== (Math::Transform<T>& transform1, Math::Transform<T>& transform2)
	{
		return CheckEquals(transform1, transform2);
	}
	
	template<typename T>
	Transform<T> Transform<T>::MakeTranslation(const T x, const T y, const T z)
	{
		Transform<T> translation = Transform<T>::Identity();
		translation.SetOriginalValueAt(0, 3, x);
		translation.SetOriginalValueAt(1, 3, y);
		translation.SetOriginalValueAt(2, 3, z);

		return translation;
	}

	template<typename T>
	Transform<T> Transform<T>::MakeScaling(const T x, const T y, const T z)
	{
		Transform<T> scaling = Transform<T>::Identity();
		scaling.SetOriginalValueAt(0, 0, x);
		scaling.SetOriginalValueAt(1, 1, y);
		scaling.SetOriginalValueAt(2, 2, z);
		return scaling;
	}

	template<typename T>
	Transform<T> Transform<T>::MakeRotation(const T radiansX, const T radiansY, const T radiansZ)
	{		
		return MakeRotationCompound(radiansX, radiansY, radiansZ);
		//return MakeRotationInplace(radiansX, radiansY, radiansZ);
	}

	template<typename T>
	Transform<T> Transform<T>::MakeShearing(
		const T xOverY, const T xOverZ,
		const T yOverX, const T yOverZ,
		const T zOverX, const T zOverY)
	{
		Transform<T> shearing = Transform<T>::Identity();
		shearing.SetOriginalValueAt(0, 1, xOverY);
		shearing.SetOriginalValueAt(0, 2, xOverZ);

		shearing.SetOriginalValueAt(1, 0, yOverX);
		shearing.SetOriginalValueAt(1, 2, yOverZ);

		shearing.SetOriginalValueAt(2, 0, zOverX);
		shearing.SetOriginalValueAt(2, 1, zOverY);

		return shearing;
	}

	template<typename T>
	void Transform<T>::SetTranslation(const T x, const T y, const T z)
	{

	}

	template<typename T>
	void Transform<T>::SetRotation(const T angleX, const T angleY, const T angleZ)
	{

	}

	template<typename T>
	Transform<T> GetTranslation()
	{

	}

	template<typename T>
	Transform<T> GetRotation()
	{

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
			auto sqr2Over2 = 0.70710678118f;

			auto pointX = H::MakePoint<float>(0.0f, 1.0f, 0.0f);
			auto pointY = H::MakePoint<float>(0.0f, 0.0f, 1.0f);
			auto pointZ = H::MakePoint<float>(0.0f, 1.0f, 0.0f);

			//rotation around the X axis
			//90*
			auto rotation = MakeRotationCompound(GetPiBy2<float>(), 0.0f, 0.0f);
			auto rotationInplace = MakeRotationInplace(GetPiBy2<float>(), 0.0f, 0.0f);
			Assert::IsTrue(rotation == rotationInplace);

			auto pointRotated = pointX * rotation;
			auto pointRotatedExpectation = H::MakePoint<float>(0.0f, 0.0f, 1.0f);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);
			
			//45*d
			rotation = MakeRotationCompound(GetPiBy4<float>(), 0.0f, 0.0f);
			rotationInplace = MakeRotationInplace(GetPiBy4<float>(), 0.0f, 0.0f);
			Assert::IsTrue(rotation == rotationInplace);

			pointRotated = pointX * rotation;
			pointRotatedExpectation = H::MakePoint<float>(0.0f, sqr2Over2, sqr2Over2);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);

			//rotation around the Y axis
			//90*
			rotation = MakeRotationCompound(0.0f, GetPiBy2<float>(), 0.0f);
			rotationInplace = MakeRotationInplace(0.0f, GetPiBy2<float>(), 0.0f);
			Assert::IsTrue(rotation == rotationInplace);

			pointRotated = pointY * rotation;
			pointRotatedExpectation = H::MakePoint<float>(1.0f, 0.0f, 0.0f);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);

			//45*d
			rotation = MakeRotationCompound(0.0f, GetPiBy4<float>(), 0.0f);
			rotationInplace = MakeRotationInplace(0.0f, GetPiBy4<float>(), 0.0f);
			Assert::IsTrue(rotation == rotationInplace);

			pointRotated = pointY * rotation;
			pointRotatedExpectation = H::MakePoint<float>(sqr2Over2, 0.0f, sqr2Over2);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);

			//rotation around the Z axis
			//90*
			rotation = MakeRotationCompound(0.0f, 0.0f, GetPiBy2<float>());
			rotationInplace = MakeRotationInplace(0.0f, 0.0f, GetPiBy2<float>());
			Assert::IsTrue(rotation == rotationInplace);

			pointRotated = pointZ * rotation;
			pointRotatedExpectation = H::MakePoint<float>(-1.0f, 0.0f, 0.0f);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);

			//45*d
			rotation = MakeRotationCompound(0.0f, 0.0f, GetPiBy4<float>());
			rotationInplace = MakeRotationInplace(0.0f, 0.0f, GetPiBy4<float>());
			Assert::IsTrue(rotation == rotationInplace);

			pointRotated = pointZ * rotation;
			pointRotatedExpectation = H::MakePoint<float>(-sqr2Over2, sqr2Over2, 0.0f);
			Assert::IsTrue(pointRotated == pointRotatedExpectation);
		}

		TEST_METHOD(Transform_Shearing)
		{
			auto point = H::MakePoint<float>(2.0f, 3.0f, 4.0f);
			//x over y
			auto shearing = Transform<float>::MakeShearing(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			auto skewnPoint = point * shearing;
			auto expectation = H::MakePoint<float>(5.0f, 3.0f, 4.0f);
			Assert::IsTrue(skewnPoint == expectation);

			//x over z
			shearing = Transform<float>::MakeShearing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			skewnPoint = point * shearing;
			expectation = H::MakePoint<float>(6.0f, 3.0f, 4.0f);
			Assert::IsTrue(skewnPoint == expectation);

			//y over x
			shearing = Transform<float>::MakeShearing(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			skewnPoint = point * shearing;
			expectation = H::MakePoint<float>(2.0f, 5.0f, 4.0f);
			Assert::IsTrue(skewnPoint == expectation);

			//y over z
			shearing = Transform<float>::MakeShearing(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			skewnPoint = point * shearing;
			expectation = H::MakePoint<float>(2.0f, 7.0f, 4.0f);
			Assert::IsTrue(skewnPoint == expectation);

			//z over x
			shearing = Transform<float>::MakeShearing(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
			skewnPoint = point * shearing;
			expectation = H::MakePoint<float>(2.0f, 3.0f, 6.0f);
			Assert::IsTrue(skewnPoint == expectation);

			//z over y
			shearing = Transform<float>::MakeShearing(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			skewnPoint = point * shearing;
			expectation = H::MakePoint<float>(2.0f, 3.0f, 7.0f);
			Assert::IsTrue(skewnPoint == expectation);
		}

		TEST_METHOD(Transform_ChainingMultipleTransforms)
		{
			auto point = H::MakePoint<float>(1.0f, 0.0f, 1.0f);
			auto rotation = Transform<float>::MakeRotation(GetPiBy2<float>(), 0.0f, 0.0f);
			auto scaling = Transform<float>::MakeScaling(5.0f, 5.0f, 5.0f);
			auto translation = Transform<float>::MakeTranslation(10.0f, 5.0f, 7.0f);

			auto expectation = H::MakePoint<float>(1.0f, -1.0f, 0.0f);
			auto pointRotated = point * rotation;
			Assert::IsTrue(pointRotated == expectation);

			expectation = H::MakePoint<float>(5.0f, -5.0f, 0.0f);
			auto pointRotatedAndScaled = pointRotated * scaling;
			Assert::IsTrue(pointRotatedAndScaled == expectation);

			expectation = H::MakePoint<float>(15.0f, 0.0f, 7.0f);
			auto pointRotatedAndScaledAndTranslated = pointRotatedAndScaled * translation;
			Assert::IsTrue(pointRotatedAndScaledAndTranslated == expectation);
		}

		TEST_METHOD(Transform_ChainingMultipleTransforms_Reverse)
		{	
			auto rotation = Transform<float>::MakeRotation(GetPiBy2<float>(), 0.0f, 0.0f);
			auto scaling = Transform<float>::MakeScaling(5.0f, 5.0f, 5.0f);
			auto translation = Transform<float>::MakeTranslation(10.0f, 5.0f, 7.0f);

			auto transformResult1 = rotation * scaling;
			auto transformResult2 = scaling * rotation;
			Assert::IsTrue(transformResult1 == transformResult2);

			transformResult1 = translation * scaling;
			transformResult2 = scaling * translation;
			Assert::IsTrue(transformResult1 == transformResult2); // is translation the culprit?

			transformResult1 = rotation * translation; 
			transformResult2 = translation * rotation;
			Assert::IsTrue(transformResult1 == transformResult2); // is translation the culprit?			
		}
	};
}
#endif