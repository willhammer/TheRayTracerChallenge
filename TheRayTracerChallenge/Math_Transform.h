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
		static auto MakeTranslation(const T x, const T y, const T z) ->
			std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>;

		static auto MakeScaling(const T x, const T y, const T z) ->
			std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>;

		static auto MakeRotationEuler(const T angleX, const T angleY, const T angleZ) ->
			std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>;

		static auto MakeShearing(
			const T xOverY, const T xOverZ, 
			const T yOverX, const T yOverZ,
			const T zOverX, const T zOverY) ->
			std::enable_if_t<std::is_floating_point_v<T>, Math::SquareMatrix<T, 4>>;

		SquareMatrix<T, 4> GetTranslation();
		SquareMatrix<T, 4> GetScaling();
		SquareMatrix<T, 4> GetRotation();
	};
	
};

