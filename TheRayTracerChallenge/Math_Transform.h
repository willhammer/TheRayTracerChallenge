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
		void SetTranslation(const T x, const T y, const T z);
		void SetRotationEuler(const T angleX, const T angleY, const T angleZ);
		
		static Math::SquareMatrix<T, 4> MakeTranslation(const T x, const T y, const T z);
		static Math::SquareMatrix<T, 4> MakeScaling(const T x, const T y, const T z);
		static Math::SquareMatrix<T, 4> MakeRotationEuler(const T angleX, const T angleY, const T angleZ);
		static Math::SquareMatrix<T, 4> MakeShearing
		(
			const T xOverY, const T xOverZ,
			const T yOverX, const T yOverZ,
			const T zOverX, const T zOverY
		);
	};
	
};

