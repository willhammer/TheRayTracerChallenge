#pragma once

#include <type_traits>
#include <limits>
#include <variant>


namespace Math
{
    template<typename T> class Tuple4;
    template<typename T> class Vector4;
    template<typename T> class Point4;
}

#define IsA(T, Y) std::is_base_of_v<T, Y>
#define Equalsf Equals<float>

namespace Math
{   
    template<typename T> class Tuple;
    template<typename T> class Vector4;
    template<typename T> class Point4;
    template<typename T> class Color4;

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

                default:
                    break;
            }
        }

        template<typename T> static Point4<T> MakePoint(const T& x, const T& y, const T& z);
        template<typename T> static Vector4<T> MakeVector(const T& x, const T& y, const T& z);

        template<typename T> static Point4<T> MakePoint(const Tuple4<T>&& tuple);
        template<typename T> static Vector4<T> MakeVector(const Tuple4<T>&& tuple);
    };
    
    using H = Math::Helpers;
    using C = Math::Helpers::Coordinate;

    template <typename T>
    struct IsComparable { constexpr static bool value = false; };

    template <typename U>
    struct IsComparable<Vector4<U>> { constexpr static bool value = true; };

    template <typename U>
    struct IsComparable<Point4<U>> { constexpr static bool value = true; };
    
    namespace M = Math;

    template<typename T> inline auto GetEpsilon() 
        -> std::enable_if_t<std::is_floating_point_v<T>, T>
    {
        return std::numeric_limits<T>::epsilon() * T(10);
    }

    template<typename T> inline auto AlmostEquals(const T& first, const T& second) 
        -> std::enable_if_t<std::is_floating_point_v<T>, T>
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

    template<typename T> M::Vector4<T> operator*(const M::Vector4<T>& vector, const T scalar);
    template<typename T> void          operator*= (M::Vector4<T>& vector, const T scalar);
    template<typename T> M::Vector4<T> operator/(const M::Vector4<T>& vector, const T scalar);
    
    template<typename T> void          operator/= (M::Vector4<T>& vector, const T scalar);
    
    template<typename T> M::Point4<T>  operator- (const M::Point4<T>& first, const M::Vector4<T>& second);
    template<typename T> M::Vector4<T> operator- (const M::Point4<T>& first, const M::Point4<T>& second);
    template<typename T> M::Vector4<T> operator- (const M::Vector4<T>& first, const M::Vector4<T>& second);

    template<template<typename> typename T, typename U> constexpr auto operator== (const T<U>& first, const T<U>& second)
        ->std::enable_if_t<IsComparable<T<U>>::value, bool>
    {
        return
            Equals<U>(H::Get(first, C::X), H::Get(second, C::X)) &&
            Equals<U>(H::Get(first, C::Y), H::Get(second, C::Y)) &&
            Equals<U>(H::Get(first, C::Z), H::Get(second, C::Z));
    }
}

namespace Math
{
    template<typename T> class Tuple;
    template<typename T> class Vector4;
    template<typename T> class Point4;
    template<typename T> class Color4;
    
    using H = Helpers;
    using C = Helpers::Coordinate;

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
        const T w = T{ 1 };

        friend class Helpers;
        using Tuple4::Tuple4;

        Point4(T x, T y, T z);
        Point4(const Tuple4<T>& input) : Point4{ 
            H::Get(input, C::X),
            H::Get(input, C::Y),
            H::Get(input, C::Z) } { }
    };

    template<typename T>
    class Vector4 : public Tuple4<T>
    {
    private:
        const T w = T{ 0 };

        friend class Helpers;
        using Tuple4::Tuple4;

        Vector4(T x, T y, T z);
        Vector4(const Tuple4<T>& input) : Vector4{ 
            H::Get(input, C::X),
            H::Get(input, C::Y),
            H::Get(input, C::Z) } { }

    public:
        inline constexpr T          GetMagnitudeSquared() const;
        inline constexpr T          GetMagnitude() const;
        inline constexpr T          Dot(const Vector4<T>& other) const;
        inline constexpr Vector4<T> Cross(const Vector4<T>& other) const;

        Vector4         GetNormalized() const;
        void            Normalize();
    };

    template<typename T>
    class Color4 : public Tuple4<T>
    {
        friend class Helpers;
        using Tuple4::Tuple4;
    };
}

