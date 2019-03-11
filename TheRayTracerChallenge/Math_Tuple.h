#pragma once

#include "Math_Common.h"

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
		static auto Get(const U<T>& tupleInput, const N value)
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
		static auto Set(U<T>& tupleInput, const N member, const T value)
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
	
	template<typename T> Math::Vector4<T>	operator*(const Math::Vector4<T>& vector, Math::SquareMatrix<T, 4>& matrix);
	template<typename T> Math::Vector4<T>	operator*(Math::SquareMatrix<T, 4>& matrix, const Math::Vector4<T>& vector);
	
	template<typename T> Math::Point4<T>	operator*(const Math::Point4<T>& point, Math::SquareMatrix<T, 4>& matrix);
	template<typename T> Math::Point4<T>	operator*(Math::SquareMatrix<T, 4>& matrix, const Math::Point4<T>& point);

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
	
	using Point4f = Math::Point4<float>;
	using Vector4f = Math::Vector4<float>;
	using Tuple4f = Math::Tuple4<float>;

}

