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
		static SquareMatrix<T, 4> MakeTranslation(const T x, const T y, const T z);
		static SquareMatrix<T, 4> MakeScaling(const T x, const T y, const T z);
		static auto MakeRotationEuler(const T angleX, const T angleY, const T angleZ) ->
			std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>;


		SquareMatrix<T, 4> GetTranslation();
		SquareMatrix<T, 4> GetScaling();
		SquareMatrix<T, 4> GetRotation();
	};
	
};

