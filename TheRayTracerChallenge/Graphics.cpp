#include "stdafx.h"
#include "Graphics.h"

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

namespace Graphics
{

}


#pragma region Graphics Tests
#ifdef _MSC_VER
namespace Graphics
{
	namespace M = Math;
	using H = M::Helpers;
	using C = M::Helpers::Coordinate;
	using CI = M::Helpers::ColorInput;
	using Tuple4f = M::Tuple4<float>;
	
	TEST_CLASS(GraphicsTest)
	{
	public:

		TEST_METHOD(CreatingCanvas)
		{
			size_t width = 10;
			size_t height = 10;

			Canvas c{ width, height };

			for (auto i = 0; i < height; ++i)
			{
				for (auto j = 0; j < width; ++j)
				{
					const auto& pixel = c.GetAt(i, j);
					Assert::AreEqual(H::Get(pixel, CI::R), 0.0f);
					Assert::AreEqual(H::Get(pixel, CI::G), 0.0f);
					Assert::AreEqual(H::Get(pixel, CI::B), 0.0f);
					Assert::AreEqual(H::Get(pixel, CI::A), 0.5f);
				}
			}
		}

		TEST_METHOD(WritingPixelsToCanvas)
		{
			size_t width = 10;
			size_t height = 10;

			Canvas c{ width, height };

			size_t specLine = 5;
			size_t specColumn = 5;

			c.SetAt(specLine, specColumn, H::MakeColor(Tuple4f{ 1.0f, 2.0f, 3.0f, 0.6f }));

			for (auto i = 0; i < height; ++i)
			{
				for (auto j = 0; j < width; ++j)
				{
					const auto& pixel = c.GetAt(i, j);

					if (i == specLine && j == specColumn)
					{
						Assert::AreEqual(H::Get(pixel, CI::R), 1.0f);
						Assert::AreEqual(H::Get(pixel, CI::G), 2.0f);
						Assert::AreEqual(H::Get(pixel, CI::B), 3.0f);
						Assert::AreEqual(H::Get(pixel, CI::A), 0.6f);
						continue;
					}
					
					Assert::AreEqual(H::Get(pixel, CI::R), 0.0f);
					Assert::AreEqual(H::Get(pixel, CI::G), 0.0f);
					Assert::AreEqual(H::Get(pixel, CI::B), 0.0f);
					Assert::AreEqual(H::Get(pixel, CI::A), 0.5f);
				}
			}



		}
	};
}
#endif
#pragma endregion