#pragma once

#include <type_traits>
#include <limits>
#include <variant>
#include <ctype.h>
#include <array>
#include <cassert>

#define IsA(T, Y) std::is_base_of_v<T, Y>
#define Equalsf Math::Equals<float>


namespace
{
	static const char* DEFAULT_ASSERT_MESSAGE = "Assert triggered";

	const std::string ConstructAssertMessage(std::string customMessage)
	{
		std::stringstream formatStream;
		formatStream << "File: " << __FILE__ << " Line: " << __LINE__ << std::endl;
		formatStream << "Error: " << customMessage << std::endl;

		return formatStream.str();
	}

	void ASSERT(bool condition, std::string message = DEFAULT_ASSERT_MESSAGE)
	{
		if (!condition)
			throw std::exception(ConstructAssertMessage(message).c_str());
	}
}


namespace Math
{
	template<typename T> class Tuple4;
	template<typename T> class Vector4;
	template<typename T> class Point4;
	template<typename T> class Color4;
	template<typename T, size_t Size> class SquareMatrix;
}

namespace
{
	template<typename T, size_t Size>
	std::array<T, Size> GetColumnFromMatrix(Math::SquareMatrix<T, Size>& matrix, size_t column);

	template<typename T, size_t Size>
	const T GetDeterminantHigherOrder(Math::SquareMatrix<T, Size>& matrix);

	template<typename T>
	const T GetDeterminant4(Math::SquareMatrix<T, 4>& matrix);

	template<typename T>
	const T GetDeterminant3(Math::SquareMatrix<T, 3>& matrix);

	template<typename T>	
	const T GetDeterminant2(Math::SquareMatrix<T, 2>& matrix);
}

namespace Math
{
	class Helpers
	{
	public:
		enum class Coordinate : unsigned char
		{
			X = 0,
			Y = 1,
			Z = 2,
			W = 3
		};

		enum class ColorInput : unsigned char
		{
			R = 0,
			G = 1,
			B = 2,
			A = 3
		};
	private:
		template <typename T>
		struct HasValidInput { constexpr static bool value = false; };

		template <>
		struct HasValidInput<ColorInput> { constexpr static bool value = true; };

		template <>
		struct HasValidInput<Coordinate> { constexpr static bool value = true; };

	public:
		template<typename N, template<typename> typename U, typename T>
		static constexpr auto Get(const U<T>& tupleInput, const N value)
			->std::enable_if_t<HasValidInput<N>::value, T>
		{
			switch (value)
			{
				case 0:
					return tupleInput.x;

				case 1:
					return tupleInput.y;

				case 2:
					return tupleInput.z;

				case 3:
					return tupleInput.w;

				default:
					return tupleInput.x;
			}
		}

		template<typename N, template<typename> typename U, typename T>
		static constexpr auto Set(U<T>& tupleInput, const N member, const T value)
			->std::enable_if_t<HasValidInput<N>::value, void>
		{
			switch (member)
			{
				case 0:
					tupleInput.x = value;
					break;

				case 1:
					tupleInput.y = value;
					break;

				case 2:
					tupleInput.z = value;
					break;

				case 3:
					tupleInput.w = value;						

				default:
					break;
			}
		}

		template<typename T> static Point4<T> MakePoint(const T& x, const T& y, const T& z);
		template<typename T> static Vector4<T> MakeVector(const T& x, const T& y, const T& z);
		template<typename T> static Color4<T> MakeColor(const T& r, const T& g, const T& b, const T& a);

		template<typename T> static Point4<T> MakePoint(const Tuple4<T>& tuple);
		template<typename T> static Vector4<T> MakeVector(const Tuple4<T>& tuple);
		template<typename T> static Color4<T> MakeColor(const Tuple4<T>& tuple);
	};
	
	template<typename T> Math::Vector4<T>	operator*(const Math::Vector4<T>& vector, const T scalar);
	template<typename T> Math::Color4<T>	operator*(const Math::Color4<T>& color, const T scalar);
	template<typename T> Math::Color4<T>	operator*(const Math::Color4<T>& color1, const Math::Color4<T>& color2);
	template<typename T, size_t Size> Math::SquareMatrix<T, Size> 
											operator*(Math::SquareMatrix<T, Size>& matrix1, Math::SquareMatrix<T, Size>& matrix2);

	template<typename T, size_t Size> Math::Vector4<T>
											operator*(const Math::Vector4<T>& vector, Math::SquareMatrix<T, Size>& matrix);

	template<typename T> void				operator*= (Math::Vector4<T>& vector, const T scalar);
	template<typename T> void				operator*= (Math::Color4<T>& color, const T scalar);
	template<typename T> void				operator*= (Math::Color4<T>& color, const Math::Color4<T>& colorOther);

	template<typename T> Math::Vector4<T>	operator/(const Math::Vector4<T>& vector, const T scalar);
	template<typename T> Math::Color4<T>	operator/(const Math::Color4<T>& color, const T scalar);

	template<typename T> void				operator/= (Math::Vector4<T>& vector, const T scalar);
	template<typename T> void				operator/= (Math::Color4<T>& color, const T scalar);

	template<typename T> Math::Point4<T>	operator+ (const Math::Point4<T>& first, const Math::Vector4<T>& second);
	template<typename T> Math::Vector4<T>	operator+ (const Math::Vector4<T>& first, const Math::Vector4<T>& second);
	template<typename T> Math::Color4<T>	operator+ (const Math::Color4<T>& first, const Math::Color4<T>& second);
		
	template<typename T> Math::Point4<T>	operator- (const Math::Point4<T>& first, const Math::Vector4<T>& second);
	template<typename T> Math::Vector4<T>	operator- (const Math::Point4<T>& first, const Math::Point4<T>& second);
	template<typename T> Math::Vector4<T>	operator- (const Math::Vector4<T>& first, const Math::Vector4<T>& second);
	template<typename T> Math::Color4<T>	operator- (const Math::Color4<T>& first, const Math::Color4<T>& second);

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
	
	template<typename T, size_t Size> constexpr bool operator== (Math::SquareMatrix<T, Size>& matrix1, Math::SquareMatrix<T, Size>& matrix2)
	{
		for (size_t indexLine = 0; indexLine < Size; ++indexLine)
		{
			for (size_t indexColumn = 0; indexColumn < Size; ++indexColumn)
			{
				if (!Equals<T>(
					matrix1.GetValueAt(indexLine, indexColumn), 
					matrix2.GetValueAt(indexLine, indexColumn)))
					return false;
			}
		}

		return true;
	}

	/* Y is up, Z points away from the camera */
	template<typename T>
	class Tuple4
	{
		friend class Point4<T>;
		friend class Vector4<T>;
		friend class Color4<T>;
		friend class Helpers;
	private:
		T x, y, z, w;

	public:
		Tuple4(T x, T y, T z, T w);
		Tuple4          GetNegated();
		void            Negate();
	};

	template<typename T>
	class Point4 : public Tuple4<T>
	{
	private:
		friend class Helpers;
		using Tuple4::Tuple4;

		Point4(T x, T y, T z);
		Point4(const Tuple4<T>& input) : Point4{ 
			Math::Helpers::Get(input, Math::Helpers::Coordinate::X),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::Y),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::Z) } { }
	public:
		Point4() : Point4(T{ 0 }, T{ 0 }, T{ 0 }) {};
	};

	template<typename T>
	class Vector4 : public Tuple4<T>
	{
	private:
		friend class Helpers;
		using Tuple4::Tuple4;

		Vector4(T x, T y, T z);
		Vector4(const Tuple4<T>& input) : Vector4{ 
			Math::Helpers::Get(input, Math::Helpers::Coordinate::X),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::Y),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::Z) } { }
	public:
		inline constexpr T          GetMagnitudeSquared() const;
		inline constexpr T          GetMagnitude() const;
		inline constexpr T          Dot(const Vector4<T>& other) const;
		inline constexpr Vector4<T> Cross(const Vector4<T>& other) const;

		Vector4         GetNormalized() const;
		void            Normalize();

	public:
		Vector4() : Vector4(T{ 0 }, T{ 0 }, T{ 0 }) {};
	};

	template<typename T>
	class Color4 : public Tuple4<T>
	{
	private:
		friend class Helpers;
		using Tuple4::Tuple4;

		Color4(T r, T g, T b, T a);
		Color4(const Tuple4<T>& input) : Color4{
			Math::Helpers::Get(input, Math::Helpers::Coordinate::X),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::Y),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::Z),
			Math::Helpers::Get(input, Math::Helpers::Coordinate::W) } { }

	public:
		Color4() : Color4(T{ 0 }, T{ 0 }, T{ 0 }, T{ 0.5 }) {};
		inline constexpr void Hadamard(const Color4<T>& other); /*Color * Color multiplication*/
	};


	template<typename T, size_t Size>
	class SquareMatrix
	{
	private: 
		friend class Helpers;

		std::array<std::array<T, Size>, Size> contents;
		bool transposed;

	public:

		static SquareMatrix Identity()
		{
			SquareMatrix<T, Size> identity;
			for (size_t i = 0; i < Size; ++i)
			{
				identity.GetValueAt(i, i) = T(1);
			}

			return identity;
		}

		SquareMatrix()
		{
			contents = std::array<std::array<T, Size>, Size>();
			transposed = false;
		}

		SquareMatrix(std::array<std::array<T, Size>, Size> contentsNew)
		{
			contents = contentsNew;
			transposed = false;
		}

		void SetOriginalValueAt(size_t line, size_t column, const T& value)
		{
			contents[line][column] = value;
		}

		T& GetValueAt(size_t line, size_t column)
		{ 
			return transposed ? contents[column][line] : contents[line][column];
		}

		T& GetOriginalValueAt(size_t line, size_t column)
		{
			return contents[line][column];
		}
		
		std::array<T, Size> GetLineAt(size_t line) 
		{ 
			return transposed ? GetColumnFromMatrix(*this, line) : contents[line];
		}
		
		std::array<T, Size> GetColumnAt(size_t column) 
		{	
			return transposed ? contents[column] : GetColumnFromMatrix(*this, column);
		}

		const T GetDeterminant() const
		{
			switch (Size)
			{
			case 0:
			case 1:
				return 0;
			case 2:
				return GetDeterminant2<T>((Math::SquareMatrix<T, 2>&)*this);
			case 3:
				return GetDeterminant3<T>((Math::SquareMatrix<T, 3>&)*this);
			case 4:
				return GetDeterminant4<T>((Math::SquareMatrix<T, 4>&)*this);
			default:
				return 0;// GetDeterminantHigherOrder<T, Size>((Math::SquareMatrix<T, Size>&)*this);
			}
		}
		
		void SetTransposed(bool setTransposed) { transposed = setTransposed; }
	};

	namespace
	{
		template<typename T, size_t Size>
		std::array<T, Size> GetColumnFromMatrix(SquareMatrix<T, Size>& matrix, size_t column)
		{
			std::array<T, Size> returnColumn;
			for (size_t i = 0; i < Size; ++i)
			{
				returnColumn[i] = matrix.GetOriginalValueAt(i, column);
			}

			return returnColumn;
		}

		template<typename T, size_t Size, size_t Subsize>
		std::array<Math::SquareMatrix<T, Subsize>, Size> FindSubmatricesOfSize(Math::SquareMatrix<T, Size>& matrix)
		{
			std::array<Math::SquareMatrix<T, Subsize>, Size>
		}

		template<typename T, size_t Size>
		const T GetDeterminantHigherOrder(Math::SquareMatrix<T, Size>& matrix)
		{
		#ifdef HIGHER_ORDER_FIXED
			const size_t subMatrixSize = Size - 1;

			std::array<Math::SquareMatrix<T, subMatrixSize>, Size> subMatrices = GetSubmatrices(matrix);
			T detVal = T(0);

			for (size_t i = 0; i < Size; ++i)
			{
				T sign = T(pow(-1.0, double(i)));
				T factor = matrix.GetOriginalValueAt(0, i);
				T subMatrixDeterminant = 0;
				
				if (Size > 3)
				{
					subMatrixDeterminant = GetDeterminantHigherOrder<T, Size - 1>(subMatrices.at(i));
				}

				else
				{
					auto matrix2 = (Math::SquareMatrix<T, 2>&) subMatrices.at(i);
					subMatrixDeterminant = GetDeterminant2(matrix2);
				}

				detVal += sign * factor * subMatrixDeterminant;
			}

			return detVal;

		#else
			return T(0);
		#endif
		}

		template<typename T, size_t Size>
		std::array<Math::SquareMatrix<T, Size - 1>, Size> GetSubmatrices(Math::SquareMatrix<T, Size>& matrix)
		{
			std::array<Math::SquareMatrix<T, Size - 1>, Size> subMatrices;
			for (size_t i = 0; i < Size; ++i)
			{
				Math::SquareMatrix<T, Size - 1> subMatrix;
				int line = 0;
				
				for (size_t lineOriginal = 1; lineOriginal < Size; ++lineOriginal)
				{
					int column = 0;
					for (size_t columnOriginal = 0; columnOriginal < Size; ++columnOriginal)
					{
						if (columnOriginal == i)
							continue;
						
						subMatrix.SetOriginalValueAt(line, column, matrix.GetOriginalValueAt(lineOriginal, columnOriginal));
						++column;
					}
					++line;
				}

				subMatrices.at(i) = subMatrix;				
			}

			return subMatrices;
		}

		template<typename T>
		const T GetDeterminant4(Math::SquareMatrix<T, 4>& matrix)
		{
			std::array<Math::SquareMatrix<T, 3>, 4> subMatrices = GetSubmatrices(matrix);
			T detVal = T(0);

			for (size_t i = 0; i < 4; ++i)
			{
				T sign = T(pow(-1.0, double(i)));
				T factor = matrix.GetOriginalValueAt(0, i);
				T subMatrixDeterminant = GetDeterminant3(subMatrices.at(i));

				detVal += sign * factor * subMatrixDeterminant;
			}

			return detVal;
		}

		template<typename T>
		const T GetDeterminant3(Math::SquareMatrix<T, 3>& matrix)
		{
		/*
			0,0  0,1  0,2
			
			1,0  1,1  1,2

			2,0  2,1  2,2		
		*/ 
			return
				matrix.GetOriginalValueAt(0, 0) *
				(	matrix.GetOriginalValueAt(1, 1) * matrix.GetOriginalValueAt(2, 2) -
					matrix.GetOriginalValueAt(1, 2) * matrix.GetOriginalValueAt(2, 1))
				- matrix.GetOriginalValueAt(0, 1) *
				(	matrix.GetOriginalValueAt(1, 0) * matrix.GetOriginalValueAt(2, 2) -
					matrix.GetOriginalValueAt(1, 2) * matrix.GetOriginalValueAt(2, 0))
				+ matrix.GetOriginalValueAt(0, 2) *
				(	matrix.GetOriginalValueAt(1, 0) * matrix.GetOriginalValueAt(2, 1) -
					matrix.GetOriginalValueAt(1, 1) * matrix.GetOriginalValueAt(2, 0));
		}


		template<typename T>
		const T GetDeterminant2(Math::SquareMatrix<T, 2>& matrix)
		{
			return 
				matrix.GetOriginalValueAt(0, 0) * matrix.GetOriginalValueAt(1, 1) -
				matrix.GetOriginalValueAt(0, 1) * matrix.GetOriginalValueAt(1, 0);
				
		}
	}

	using Point4f = Math::Point4<float>;
	using Vector4f = Math::Vector4<float>;
	using Tuple4f = Math::Tuple4<float>;
	using Matrix4f = Math::SquareMatrix<float, 4>;
}

