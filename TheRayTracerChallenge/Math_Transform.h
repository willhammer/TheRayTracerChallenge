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
		
        Transform<T> GetTranslation()
        {
            Transform<T> translation = Transform<T>::Identity();
            translation.SetOriginalValueAt(3, 0, GetValueAt(3, 0));
            translation.SetOriginalValueAt(3, 1, GetValueAt(3, 1));
            translation.SetOriginalValueAt(3, 2, GetValueAt(3, 2));
            translation.SetOriginalValueAt(3, 3, GetValueAt(3, 3));

            return translation;
        }
                
        Transform<T> GetRotation()
        {
            Transform<T> rotation = *this;
            for (size_t i = 0; i < 3; ++i)
            {
                for (size_t j = 0; j < 3; ++j)
                {
                    rotation.SetOriginalValueAt(i, j, this->GetValueAt(i, j));
                }
            }

            return rotation;
        }
                
        void SetTranslation(const Transform<T>& transformOther)
        {
            this->SetOriginalValueAt(3, 0, transformOther.GetValueAt(3, 0));
            this->SetOriginalValueAt(3, 1, transformOther.GetValueAt(3, 1));
            this->SetOriginalValueAt(3, 2, transformOther.GetValueAt(3, 2));
            this->SetOriginalValueAt(3, 3, transformOther.GetValueAt(3, 3));
        }
                
        void SetRotation(const Transform<T>& transformOther)
        {
            for (size_t i = 0; i < 3; ++i)
            {
                for (size_t j = 0; j < 3; ++j)
                {
                    this->SetOriginalValueAt(i, j, t transformOther.GetValueAt(i, j));
                }
            }
        }
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
#pragma endregion

};

