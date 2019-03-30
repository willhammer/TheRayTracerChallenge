#include "stdafx.h"
#include "Math_Matrix.h"
#include <functional>

#ifdef _MSC_VER
	#include "CppUnitTest.h"
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

namespace Math
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
		const size_t subMatrixSize = Size - 1;

		std::array<Math::SquareMatrix<T, subMatrixSize>, Size> subMatrices = GetSubmatrices(matrix);
		T detVal = T(0);

		for (size_t i = 0; i < Size; ++i)
		{
			T sign = T(pow(-1.0, double(i)));
			T factor = matrix.GetValueAt(0, i);
			T subMatrixDeterminant = GetDeterminantHigherOrder<T, subMatrixSize>(subMatrices.at(i));

			detVal += sign * factor * subMatrixDeterminant;
		}

		return detVal;
	}

	template<typename T, size_t Size>
	const T GetDeterminantHigherOrder(Math::SquareMatrix<T, 2>& matrix)
	{
		return GetDeterminant2<T>(matrix);
	}

	template<typename T, size_t Size>
	SquareMatrixArray<T, Size> GetCofactorSubmatrices(Math::SquareMatrix<T, Size>& matrix)
	{
		SquareMatrixArray<T, Size> cofactorSubmatrices;

		for (size_t line = 0; line < Size; ++line)
		{
			for (size_t column = 0; column < Size; ++column)
			{
				SquareMatrix<T, Size - 1> cofactorSubmatrix;

				size_t lineInSubmatrix = 0;
				for (size_t lineOriginal = 0; lineOriginal < Size; ++lineOriginal)
				{
					size_t columnInSubmatrix = 0;
					if (lineOriginal == line)
						continue;

					for (size_t columnOriginal = 0; columnOriginal < Size; ++columnOriginal)
					{
						if (columnOriginal == column)
							continue;

						cofactorSubmatrix.SetValueAt(
							lineInSubmatrix,
							columnInSubmatrix,
							matrix.GetValueAt(lineOriginal, columnOriginal));

						++columnInSubmatrix;
					}
					++lineInSubmatrix;
				}
				cofactorSubmatrices[line][column] = cofactorSubmatrix;
			}
		}

		return cofactorSubmatrices;
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

					subMatrix.SetOriginalValueAt(line, column, matrix.GetValueAt(lineOriginal, columnOriginal));
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
			T sign = i % 2 == 0 ? T(1) : T(-1);
			T factor = matrix.GetOriginalValueAt(0, i);
			T subMatrixDeterminant = GetDeterminant3(subMatrices.at(i));

			detVal += sign * factor * subMatrixDeterminant;
		}

		return detVal;
	}

	template<typename T>
	const T GetDeterminant3(Math::SquareMatrix<T, 3>& matrix)
	{
		return
			matrix.GetOriginalValueAt(0, 0) *
			(matrix.GetOriginalValueAt(1, 1) * matrix.GetOriginalValueAt(2, 2) -
				matrix.GetOriginalValueAt(1, 2) * matrix.GetOriginalValueAt(2, 1))
			- matrix.GetOriginalValueAt(0, 1) *
			(matrix.GetOriginalValueAt(1, 0) * matrix.GetOriginalValueAt(2, 2) -
				matrix.GetOriginalValueAt(1, 2) * matrix.GetOriginalValueAt(2, 0))
			+ matrix.GetOriginalValueAt(0, 2) *
			(matrix.GetOriginalValueAt(1, 0) * matrix.GetOriginalValueAt(2, 1) -
				matrix.GetOriginalValueAt(1, 1) * matrix.GetOriginalValueAt(2, 0));
	}

	template<typename T>
	const T GetDeterminant2(Math::SquareMatrix<T, 2>& matrix)
	{
		return
			matrix.GetOriginalValueAt(0, 0) * matrix.GetOriginalValueAt(1, 1) -
			matrix.GetOriginalValueAt(0, 1) * matrix.GetOriginalValueAt(1, 0);

	}

	template<typename T, typename Functor, size_t Size>
	SquareMatrixContents<T, Size> GetContentsAfterOperation(
		const SquareMatrixContents<T, Size>& first,
		const SquareMatrixContents<T, Size>& second)
	{
		SquareMatrixContents<T, Size> retVal;
		Functor operation;

		for (size_t lineIndex = 0; lineIndex < Size; ++lineIndex)
		{
			for (size_t columnIndex = 0; columnIndex < Size; ++columnIndex)
			{
				retVal[lineIndex][columnIndex] = 0;
				T valueToSet = 0;

				for (size_t indexMul = 0; indexMul < Size; ++indexMul)
				{
					valueToSet += operation(first[lineIndex][indexMul], second[indexMul][columnIndex]);
				}

				retVal[lineIndex][columnIndex] = valueToSet;
			}
		}

		return retVal;
	}


	template<typename T, size_t Size>
	SquareMatrixContents<T, Size> GetAddedContents(const SquareMatrixContents<T, Size>& first, const SquareMatrixContents<T, Size>& second)
	{
		return GetContentsAfterOperation<T, std::plus<T>, Size>(first, second);
	}
	
	template<typename T, size_t Size>
	SquareMatrixContents<T, Size> GetMultipliedContents(const SquareMatrixContents<T, Size>& first, const SquareMatrixContents<T, Size>& second)
	{
		return GetContentsAfterOperation<T, std::multiplies<T>, Size>(first, second);
	}

	template<typename T, size_t Size>
	bool CheckEquals(const SquareMatrixContents<T, Size>& first, const SquareMatrixContents<T, Size>& second)
	{
		auto epsilon = GetEpsilon<T>();

		for (size_t indexLine = 0; indexLine < Size; ++indexLine)
		{
			for (size_t indexColumn = 0; indexColumn < Size; ++indexColumn)
			{
				auto value1 = first[indexLine][indexColumn];
				auto value2 = second[indexLine][indexColumn];

				if (!Equals<T>(value1, value2))
					return false;
			}
		}

		return true;
	}

	template<typename T, size_t Size>
	SquareMatrixContents<T, Size> GetZero()
	{
		SquareMatrixContents<T, Size> zero;
		for (size_t i = 0; i < Size; ++i)
			for (size_t j = 0; j < Size; ++j)
				zero[i][j] = T(0);

		return zero;
	}

	template<typename T, size_t Size>
	SquareMatrixContents<T, Size> GetIdentity()
	{
		SquareMatrixContents<T, Size> identity = GetZero<T, Size>();
		for (size_t i = 0; i < Size; ++i)
		{
			identity[i][i] = T(1);
		}

		return identity;
	}

	template<typename T, size_t Size> 
	bool operator== (
		Math::SquareMatrix<T, Size>& matrix1, 
		Math::SquareMatrix<T, Size>& matrix2)
	{
		return CheckEquals<T, Size>(matrix1.GetContents(), matrix2.GetContents());
	}

	template<typename T, size_t Size>
	SquareMatrix<T, Size> operator*(SquareMatrix<T, Size>& matrix1, SquareMatrix<T, Size>& matrix2)
	{
		return Math::SquareMatrix<T, Size>(GetMultipliedContents(matrix1.GetContents(), matrix2.GetContents()));		
	}

	template<typename T, size_t Size>
	Math::SquareMatrix<T, Size>	operator+(
		Math::SquareMatrix<T, Size>& matrix1,
		Math::SquareMatrix<T, Size>& matrix2)
	{
		return Math::SquareMatrix<T, Size>(GetAddedContents(matrix1.GetContents(), matrix2.GetContents()));		
	}
}

#pragma region Math Tests
#ifdef _MSC_VER
namespace Math
{
	TEST_CLASS(TestMathMatrix)
	{

	public:

		TEST_METHOD(MatrixCreation)
		{
			auto matrix1 = SquareMatrix<float, 2>({ 1.0f, 2.0f, 3.0f, 4.0f });

			Assert::IsTrue(Equals<float>(matrix1.GetValueAt(0, 0), 1.0f));
			Assert::IsTrue(Equals<float>(matrix1.GetValueAt(0, 1), 2.0f));
			Assert::IsTrue(Equals<float>(matrix1.GetValueAt(1, 0), 3.0f));
			Assert::IsTrue(Equals<float>(matrix1.GetValueAt(1, 1), 4.0f));
		}

		TEST_METHOD(MatrixGetLine)
		{
			auto matrix = SquareMatrix<float, 3>({
				1.0f, 2.0f, 4.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto line = matrix.GetLineAt(0);
			auto expected = std::array<float, 3>{1.0f, 2.0f, 4.0f};

			Assert::IsTrue(line == expected);

			matrix.SetTransposed(true);
			line = matrix.GetLineAt(0);
			expected = std::array<float, 3>{1.0f, 4.0f, 7.0f};
			Assert::IsTrue(line == expected);
		}

		TEST_METHOD(MatrixGetColumn)
		{
			auto matrix = SquareMatrix<float, 3>({
				1.0f, 2.0f, 4.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto line = matrix.GetColumnAt(0);
			auto expected = std::array<float, 3>{1.0f, 4.0f, 7.0f};

			Assert::IsTrue(line == expected);

			matrix.SetTransposed(true);
			line = matrix.GetColumnAt(0);
			expected = std::array<float, 3>{1.0f, 2.0f, 4.0f};
			Assert::IsTrue(line == expected);
		}

		TEST_METHOD(MatrixIdentity)
		{
			auto matrix = SquareMatrix<float, 4>::Identity();

			auto expectedResult = SquareMatrix<float, 4>({
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f });

			Assert::IsTrue(expectedResult == matrix);
		}

		TEST_METHOD(MatrixEquality)
		{
			auto matrix1 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto matrix2 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto matrix3 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 4.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			Assert::IsTrue(matrix1 == matrix2);
			Assert::IsFalse(matrix1 == matrix3);
		}

		TEST_METHOD(MatrixAddition)
		{
			auto matrix1 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto matrix2 = SquareMatrix<float, 3>({
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f });

			auto matrixExpectation = SquareMatrix<float, 3>({
				2.0f, 2.0f, 3.0f,
				4.0f, 6.0f, 6.0f,
				7.0f, 8.0f, 10.0f });

			auto matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrixExpectation);
		}

		TEST_METHOD(MatrixMultiplication)
		{
			auto matrix1 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			auto matrix2 = SquareMatrix<float, 3>({
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f });

			auto matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrix1);

			matrix2 = SquareMatrix<float, 3>({
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f });

			auto matrixExpectation = SquareMatrix<float, 3>({
				6.0f, 6.0f, 6.0f,
				15.0f, 15.0f, 15.0f,
				24.0f, 24.0f, 24.0f });

			matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrixExpectation);
		}

		TEST_METHOD(MatrixMultiplicationTransposed)
		{
			auto matrix1 = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				7.0f, 8.0f, 9.0f });

			matrix1.SetTransposed(true);

			auto matrix2 = SquareMatrix<float, 3>({
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f });

			auto matrixResult = matrix1 * matrix2;
			auto matrixExpectation = SquareMatrix<float, 3>({
				1.0f, 4.0f, 7.0f,
				2.0f, 5.0f, 8.0f,
				3.0f, 6.0f, 9.0f });

			Assert::IsTrue(matrixResult == matrixExpectation);

			matrix2 = SquareMatrix<float, 3>({
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f });

			matrixExpectation = SquareMatrix<float, 3>({
				12.0f, 12.0f, 12.0f,
				15.0f, 15.0f, 15.0f,
				18.0f, 18.0f, 18.0f });

			matrixResult = matrix1 * matrix2;
			Assert::IsTrue(matrixResult == matrixExpectation);
		}

		TEST_METHOD(MatrixDeterminant_Helpers_Order2)
		{
			auto matrix = SquareMatrix<float, 2>({
				1.0f, 2.0f,
				4.0f, 5.0f });

			Assert::IsTrue(Equals<float>(matrix.GetDeterminant(), -3.0f));
		}

		TEST_METHOD(MatrixDeterminant_Helpers_Order3)
		{
			auto matrix = SquareMatrix<float, 3>({
				1.0f, 2.0f, 3.0f,
				4.0f, 5.0f, 6.0f,
				2.0f, 4.0f, 1.0f });

			Assert::IsTrue(Equals<float>(matrix.GetDeterminant(), 15.0f));
		}

		TEST_METHOD(MatrixDeterminant_Helpers_Order4)
		{
			auto matrix = SquareMatrix<float, 4>({
				11.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				4.0f, 3.0f, 2.0f, 15.0f });

			auto det = matrix.GetDeterminant();
			Assert::IsTrue(Equals<float>(det, -560.0f));

			matrix = SquareMatrix<float, 4>({
				6.0f, 4.0f, 4.0f, 4.0f,
				5.0f, 5.0f, 7.0f, 6.0f,
				4.0f, -9.0f, 3.0f, -7.0f,
				9.0f, 1.0f, 7.0f, -6.0f });

			det = matrix.GetDeterminant();
			Assert::IsTrue(Equals<float>(det, -2120.0f));

			matrix = SquareMatrix<float, 4>({
				-4.0f, 2.0f, -2.0f, -3.0f,
				9.0f, 6.0f, 2.0f, 6.0f,
				0.0f, -5.0f, 1.0f, -5.0f,
				0.0f, 0.0f, 0.0f, 0.0f });

			det = matrix.GetDeterminant();
			Assert::IsTrue(Equals<float>(det, 0.0f));
		}

		TEST_METHOD(MatrixDeterminant_Helpers_OrderHigherThan4)
		{
			auto matrix = SquareMatrix<float, 5>({
				11.0f,	2.0f,	3.0f,	4.0f,	5.0f,
				5.0f,	6.0f,	7.0f,	8.0f,	12.0f,
				9.0f,	10.0f,	11.0f,	12.0f,	2.0f,
				4.0f,	3.0f,	2.0f,	1.0f,	3.0f,
				21.0f,	15.0f,	1.0f,	7.0f,	10.0f });

			Assert::IsTrue(Equals<float>(matrix.GetDeterminant(), 30000.0f));
		}

		TEST_METHOD(Matrix4_Submatrices)
		{
			auto matrix = SquareMatrix<float, 4>({
				11.0f, 2.0f, 3.0f, 4.0f,
				5.0f, 6.0f, 7.0f, 8.0f,
				9.0f, 10.0f, 11.0f, 12.0f,
				4.0f, 3.0f, 2.0f, 15.0f });

			auto subMatrices = GetSubmatrices(matrix);

			auto expectation = SquareMatrix<float, 3>({
				6.0f, 7.0f, 8.0f,
				10.0f, 11.0f, 12.0f,
				3.0f, 2.0f, 15.0f });
			Assert::IsTrue(subMatrices[0] == expectation);

			expectation = SquareMatrix<float, 3>({
				5.0f, 7.0f, 8.0f,
				9.0f, 11.0f, 12.0f,
				4.0f, 2.0f, 15.0f });
			Assert::IsTrue(subMatrices[1] == expectation);

			expectation = SquareMatrix<float, 3>({
				5.0f, 6.0f, 8.0f,
				9.0f, 10.0f, 12.0f,
				4.0f, 3.0f, 15.0f });
			Assert::IsTrue(subMatrices[2] == expectation);

			expectation = SquareMatrix<float, 3>({
				5.0f, 6.0f, 7.0f,
				9.0f, 10.0f, 11.0f,
				4.0f, 3.0f, 2.0f });
			Assert::IsTrue(subMatrices[3] == expectation);
		}

		TEST_METHOD(Inverse_GetCofactorMatrices)
		{
			auto matrix = SquareMatrix<float, 3>({
				5.0f, 6.0f, 7.0f,
				9.0f, 10.0f, 11.0f,
				4.0f, 3.0f, 2.0f });

			SquareMatrixArray<float, 3> cofactorMatrices = GetCofactorSubmatrices<float, 3>(matrix);

			SquareMatrixArray<float, 3> expectation =
			{
				SquareMatrix<float, 2>({
					10.0f, 11.0f,
					3.0f, 2.0f}),

				SquareMatrix<float, 2>({
					9.0f, 11.0f,
					4.0f, 2.0f}),

				SquareMatrix<float, 2>({
					9.0f, 10.0f,
					4.0f, 3.0f}),

				SquareMatrix<float, 2>({
					6.0f, 7.0f,
					3.0f, 2.0f}),

				SquareMatrix<float, 2>({
					5.0f, 7.0f,
					4.0f, 2.0f}),

				SquareMatrix<float, 2>({
					5.0f, 6.0f,
					4.0f, 3.0f}),

				SquareMatrix<float, 2>({
					6.0f, 7.0f,
					10.0f, 11.0f}),

				SquareMatrix<float, 2>({
					5.0f, 7.0f,
					9.0f, 11.0f}),

				SquareMatrix<float, 2>({
					5.0f, 6.0f,
					9.0f, 10.0f})
			};

			for (size_t i = 0; i < 3; ++i)
			{
				for (size_t j = 0; j < 3; ++j)
				{
					Assert::IsTrue(expectation[i][j] == cofactorMatrices[i][j]);
				}
			}
		}

		TEST_METHOD(Inverse_GetCofactors)
		{
			auto matrix = SquareMatrix<float, 3>({
				5.0f, 6.0f, 7.0f,
				9.0f, 10.0f, 11.0f,
				4.0f, 3.0f, 2.0f });

			SquareMatrix<float, 3> cofactors = matrix.GetCofactors();

			SquareMatrix<float, 3> expectation({
				-13.0f, 26.0f, -13.0f,
				9.0f, -18.0f, 9.0f,
				-4.0f, 8.0f, -4.0f });

			for (size_t i = 0; i < 3; ++i)
			{
				for (size_t j = 0; j < 3; ++j)
				{
					Assert::IsTrue(Equals<float>(expectation.GetOriginalValueAt(i, j), cofactors.GetOriginalValueAt(i, j)));
				}
			}
		}

		TEST_METHOD(Inverse_Matrix4_Identity)
		{
			auto matrix = SquareMatrix<float, 4>({
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f });

			auto matrixInverse = matrix.GetInverse();
			Assert::IsTrue(matrixInverse == matrix);
		}

		TEST_METHOD(Inverse_Matrix4_Test1)
		{
			auto expectation = SquareMatrix<float, 4>({
				 0.21804511f,  0.45112781f,  0.24060150f, -0.04511278f,
				-0.80827067f, -1.45676691f, -0.44360902f,  0.52067669f,
				-0.07894736f, -0.22368421f, -0.05263157f,  0.19736842f,
				-0.52255639f, -0.81390977f, -0.30075187f,  0.30639097f });

			auto matrix = SquareMatrix<float, 4>({
				-5.0f, 2.0f, 6.0f, -8.0f,
				1.0f, -5.0f, 1.0f, 8.0f,
				7.0f, 7.0f, -6.0f, -7.0f,
				1.0f, -3.0f, 7.0f, 4.0f });

			auto matrixInverse = matrix.GetInverse();
			Assert::IsTrue(matrixInverse == expectation);

			Assert::IsTrue(matrix * matrixInverse == SquareMatrix<float, 4>::Identity());

		}

		TEST_METHOD(Inverse_Matrix4_Test2)
		{
			auto matrix = SquareMatrix<float, 4>({
				 8.0f, -5.0f,  9.0f,  2.0f,
				 7.0f,  5.0f,  6.0f,  1.0f,
				-6.0f,  0.0f,  9.0f,  6.0f,
				-3.0f,  0.0f, -9.0f, -4.0f });

			auto expectation = SquareMatrix<float, 4>({
				-0.153846153f, -0.153846153f, -0.282051282f, -0.538461538f,
				-0.076923076f,  0.123076923f,  0.025641025f,  0.030769230f,
				 0.358974358f,  0.358974358f,  0.435897435f,  0.923076923f,
				-0.692307692f, -0.692307692f, -0.769230769f, -1.923076923f });

			auto matrixInverse = matrix.GetInverse();
			Assert::IsTrue(matrixInverse == expectation);
			Assert::IsTrue(matrix * matrixInverse == SquareMatrix<float, 4>::Identity());
		}

		TEST_METHOD(Inverse_Matrix4_Test3)
		{
			auto matrix = SquareMatrix<float, 4>({
				 9.0f,  3.0f,  0.0f,  9.0f,
				-5.0f, -2.0f, -6.0f, -3.0f,
				-4.0f,  9.0f,  6.0f,  4.0f,
				-7.0f,  6.0f,  6.0f,  2.0f });

			auto expectation = SquareMatrix<float, 4>({
				-0.040740740f, -0.077777777f,  0.144444444f, -0.222222222f,
				-0.077777777f,  0.033333333f,  0.366666666f, -0.333333333f,
				-0.029012345f, -0.146296296f, -0.109259259f,  0.129629629f,
				 0.177777777f,  0.066666666f, -0.266666666f,  0.333333333f });

			auto matrixInverse = matrix.GetInverse();
			Assert::IsTrue(matrixInverse == expectation);
			Assert::IsTrue(matrix * matrixInverse == SquareMatrix<float, 4>::Identity());
		}

	};
}
#endif