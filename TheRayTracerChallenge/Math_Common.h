#pragma once

#include <type_traits>
#include <limits>
#include <variant>
#include <ctype.h>
#include <array>
#include <cassert>

#define IsA(T, Y) std::is_base_of_v<T, Y>
#define Equalsf Math::Equals<float>
#define Equalsd Math::Equals<double>
#define PiFloat Math::GetPi<float>()
#define PiDouble Math::GetPi<double>()

namespace Math
{
	template<typename T> class Tuple4;
	template<typename T> class Vector4;
	template<typename T> class Point4;
	template<typename T> class Color4;
	template<typename T, size_t Size> class SquareMatrix;
	template<typename T, size_t Size>
	using SquareMatrixArray = std::array<std::array<SquareMatrix<T, Size - 1>, Size>, Size>;


	template <typename T>
	struct IsComparable { constexpr static bool value = false; };

	template <typename U>
	struct IsComparable<Vector4<U>> { constexpr static bool value = true; };

	template <typename U>
	struct IsComparable<Point4<U>> { constexpr static bool value = true; };

	template <typename U>
	struct IsComparable<Color4<U>> { constexpr static bool value = true; };

	template<typename T> inline auto GetEpsilon()
		-> std::enable_if_t<std::is_floating_point_v<T>, T>
	{
		return std::numeric_limits<T>::epsilon() * T(10);
	}

	template<typename T> 
	constexpr auto GetPi() -> std::enable_if_t<std::is_floating_point_v<T>, T>
	{
		static const T pi = T(3.141592653589793238462643383279502884197169399375105820974944592307816406286);
		return pi;
	}

	template<typename T>
	constexpr auto Get4Pi() -> std::enable_if_t<std::is_floating_point_v<T>, T> 
	{ return GetPi<T>() * T(4); }

	template<typename T>
	constexpr auto Get3Pi() -> std::enable_if_t<std::is_floating_point_v<T>, T> 
	{ return GetPi<T>() * T(3); }

	template<typename T>
	constexpr auto Get2Pi() -> std::enable_if_t<std::is_floating_point_v<T>, T> 
	{ return GetPi<T>() * T(2); }
	
	template<typename T>
	constexpr auto GetPiBy2() -> std::enable_if_t<std::is_floating_point_v<T>, T> 
	{ return GetPi<T>() * T(0.5); }
	
	template<typename T>
	constexpr auto GetPiBy4() -> std::enable_if_t<std::is_floating_point_v<T>, T> 
	{ return GetPi<T>() * T(0.25); }

	template<typename T>
	constexpr auto GetPiBy6() -> std::enable_if_t<std::is_floating_point_v<T>, T> 
	{ return GetPi<T>() / T(6); }

	template<typename T> inline auto AlmostEquals(const T& first, const T& second)
		-> std::enable_if_t<std::is_floating_point_v<T>, bool>
	{
		T diff = first - second;
		if (diff < T(0))
			diff = -diff;

		return diff < GetEpsilon<T>();
	}

	template<typename T> inline bool Equals(const T& first, const T& second)
	{
		return std::is_floating_point_v<T>
			? AlmostEquals<T>(first, second)
			: first == second;
	}

	template<template<typename> typename T, typename U> constexpr auto operator== (const T<U>& first, const T<U>& second)
		->std::enable_if_t<IsComparable<T<U>>::value, bool>
	{
		return
			Equals<U>(Math::Helpers::Get(first, Math::Helpers::Coordinate::X), Math::Helpers::Get(second, Math::Helpers::Coordinate::X)) &&
			Equals<U>(Math::Helpers::Get(first, Math::Helpers::Coordinate::Y), Math::Helpers::Get(second, Math::Helpers::Coordinate::Y)) &&
			Equals<U>(Math::Helpers::Get(first, Math::Helpers::Coordinate::Z), Math::Helpers::Get(second, Math::Helpers::Coordinate::Z));
	}


}