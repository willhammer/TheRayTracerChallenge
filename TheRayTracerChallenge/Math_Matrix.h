#pragma once
#include "Math_Common.h"

namespace Math
{
	template<typename T, size_t Size> class SquareMatrix;
	template<typename T, size_t Size> using SquareMatrixArray = std::array<std::array<SquareMatrix<T, Size - 1>, Size>, Size>;

	template<typename T, size_t Size>
	std::array<T, Size> GetColumnFromMatrix(Math::SquareMatrix<T, Size>& matrix, size_t column);

	template<typename T, size_t Size>
	SquareMatrixArray<T, Size> GetCofactorSubmatrices(Math::SquareMatrix<T, Size>& matrix);	

	template<typename T, size_t Size>
	const T GetDeterminantHigherOrder(Math::SquareMatrix<T, Size>& matrix);

	template<typename T>
	const T GetDeterminant4(Math::SquareMatrix<T, 4>& matrix);

	template<typename T>
	const T GetDeterminant3(Math::SquareMatrix<T, 3>& matrix);

	template<typename T>
	const T GetDeterminant2(Math::SquareMatrix<T, 2>& matrix);

	template<typename T, size_t Size> 
	class SquareMatrix;
	
	template<typename T, size_t Size>
	using SquareMatrixArray = std::array<std::array<SquareMatrix<T, Size - 1>, Size>, Size>;

	template<typename T, size_t Size> 
	Math::SquareMatrix<T, Size>	operator*(Math::SquareMatrix<T, Size>& matrix1, Math::SquareMatrix<T, Size>& matrix2);

	template<typename T, size_t Size> 
	bool operator== (Math::SquareMatrix<T, Size>& matrix1, Math::SquareMatrix<T, Size>& matrix2);

	template<typename T, size_t Size>
	class SquareMatrix
	{
	private:
		friend class Helpers;

		std::array<std::array<T, Size>, Size> contents;
		bool transposed;
		T determinant;
		std::array<std::array<T, Size>, Size> contentsInverse;
		bool isInverseComputed;
		bool isDeterminantComputed;

		T& GetValueInternal(size_t line, size_t column)
		{
			return transposed ? contents[column][line] : contents[line][column];
		}

	public:

		static SquareMatrix Identity()
		{
			SquareMatrix<T, Size> identity;
			for (size_t i = 0; i < Size; ++i)
			{
				identity.SetOriginalValueAt(i, i, T(1));
			}

			return identity;
		}

		SquareMatrix()
		{
			contents = std::array<std::array<T, Size>, Size>();
			transposed = false;
			determinant = T(0);
			isDeterminantComputed = false;
			isInverseComputed = false;
		}

		SquareMatrix(std::array<std::array<T, Size>, Size> contentsNew) : SquareMatrix()
		{
			contents = contentsNew;
			transposed = false;
		}

		void SetOriginalValueAt(size_t line, size_t column, const T& value)
		{
			isDeterminantComputed = false;
			isInverseComputed = false;

			contents[line][column] = value;
		}

		void SetValueAt(size_t line, size_t column, const T& value)
		{
			isDeterminantComputed = false;
			isInverseComputed = false;

			transposed ? SetOriginalValueAt(column, line, value) : SetOriginalValueAt(line, column, value);
		}

		const T& GetValueAt(size_t line, size_t column)
		{
			return transposed ? contents[column][line] : contents[line][column];
		}

		const T& GetOriginalValueAt(size_t line, size_t column)
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

		SquareMatrix<T, Size> GetCofactors()
		{
			SquareMatrix<T, Size> cofactorValues;

			const size_t subMatrixSize = Size - 1;
			SquareMatrixArray<T, Size> cofactorSubmatrices = GetCofactorSubmatrices<T, Size>(*this);

			for (size_t line = 0; line < Size; ++line)
			{
				for (size_t column = 0; column < Size; ++column)
				{
					T sign = (line + column) % 2 == 0 ? T(1) : T(-1);
					T value = cofactorSubmatrices[line][column].GetDeterminant();
					cofactorValues.SetValueAt(line, column, value * sign);
				}
			}

			return cofactorValues;
		}

		const T GetDeterminant()
		{
			if (isDeterminantComputed)
				return determinant;

			T detVal = T(0);
			switch (Size)
			{
			case 0:
			case 1:
				detVal = 0;
				break;
			case 2:
				detVal = GetDeterminant2<T>((Math::SquareMatrix<T, 2>&)*this);
				break;
			case 3:
				detVal = GetDeterminant3<T>((Math::SquareMatrix<T, 3>&)*this);
				break;
			case 4:
				detVal = GetDeterminant4<T>((Math::SquareMatrix<T, 4>&)*this);
				break;
			default:
				detVal = GetDeterminantHigherOrder<T, 5>((Math::SquareMatrix<T, 5>&)*this);
				break;
			}

			determinant = detVal;
			isDeterminantComputed = true;

			return detVal;
		}

		SquareMatrix<T, Size> GetInverse()
		{
			if (!IsInvertible())
			{
				isInverseComputed = false;
				return Identity();
			}

			if (isInverseComputed)
				return SquareMatrix<T, Size>(contentsInverse);

			SquareMatrix<T, Size> inverse;
			SquareMatrix<T, Size> cofactors = GetCofactors();
			T determinant = GetDeterminant();

			for (size_t line = 0; line < Size; ++line)
			{
				for (size_t column = 0; column < Size; ++column)
				{
					auto cofactorValue = cofactors.GetValueAt(line, column);
					contentsInverse[column][line] = cofactorValue / determinant;
				}
			}

			isInverseComputed = true;
			return SquareMatrix<T, Size>(contentsInverse);
		}

		void SetTransposed(bool setTransposed) { transposed = setTransposed; }
		bool IsInvertible()
		{
			T det = GetDeterminant();
			return !Equals<T>(det, T(0));
		}
	};
}

using Matrix4f = Math::SquareMatrix<float, 4>;