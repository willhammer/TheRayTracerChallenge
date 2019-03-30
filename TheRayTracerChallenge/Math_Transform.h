#pragma once

#include "stdafx.h"
#include "Math_Common.h"
#include "Math_Matrix.h"
#include "Math_Tuple.h"

namespace Math
{
	template<typename T>
	class Transform : public SquareMatrix<T, 4>
	{
	public:

		static Transform Zero()
		{
			Transform<T> zero;
			zero.SetContents(GetZero<T, 4>());
			return zero;
		}

		static Transform Identity()
		{
			Transform<T> identity;
			identity.SetContents(GetIdentity<T, 4>());
			return identity;
		}


		void SetTranslation(const T x, const T y, const T z);
		void SetRotation(const T angleX, const T angleY, const T angleZ);
		
		Transform<T> GetTranslation();
		Transform<T> GetRotation();

		static Transform<T> MakeTranslation(const T x, const T y, const T z);
		static Transform<T> MakeScaling(const T x, const T y, const T z);
		static Transform<T> MakeRotation(const T angleX, const T angleY, const T angleZ);
		static Transform<T> MakeShearing
		(
			const T xOverY, const T xOverZ,
			const T yOverX, const T yOverZ,
			const T zOverX, const T zOverY
		);

		void operator=(const Transform<T>& other)
		{
			this->contents = other.contents;
		}		

		Transform() : SquareMatrix()
		{
		}

		Transform(std::array<std::array<T, 4>, 4> contentsNew) : SquareMatrix(contentsNew)
		{	
		}

		Transform(const Transform& other) : SquareMatrix(other.contents)
		{
		}
	};

#pragma region operators
	template<typename T, size_t Size>
	Transform<T> operator*(Transform<T>& transform1, Transform<T>& transform2);

	template<typename T, size_t Size>
	Math::Transform<T>	operator+(Math::Transform<T>& transform1, Math::Transform<T>& transform2);

	template<typename T, size_t Size>
	bool operator== (Math::Transform<T>& transform1, Math::Transform<T>& transform2);
#pragma endregion
};

