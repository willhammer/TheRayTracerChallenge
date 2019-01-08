#include "stdafx.h"
#include "Graphics.h"

#include <sstream>

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

namespace Graphics
{


	Canvas::Canvas(size_t setWidth, size_t setHeight) : width{ setWidth }, height{ setHeight }
	{
		contents.resize(width * height);
	}

	Color4f Canvas::GetAt(size_t line, size_t column)
	{
		//how do I properly assert on the size of line and column? 
		//I cannot statically check this, so I can't use enable_if, nor static_assert
		return contents.at(line * width + column);
	}

	void Canvas::SetAt(size_t line, size_t column, Color4f value)
	{
		contents.at(line * width + column) = value;
	}

	void Canvas::SetFilename(std::string setFilename)
	{
		filename = setFilename;
	}


	void Canvas::WritePPMHeader(std::ostream& ofs)
	{
		char buff[20];

		snprintf(buff, sizeof(buff), "P3");
		ofs << buff << std::endl;

		snprintf(buff, sizeof(buff), "%zd %zd", width, height);
		ofs << buff << std::endl;

		snprintf(buff, sizeof(buff), "255");
		ofs << buff << std::endl;
	}

	void Canvas::WritePPMBody(std::ostream& ofs)
	{

	}

	std::vector<std::string> Canvas::ReadPPMHeaderRaw(std::istream& ifs)
	{
		std::vector<std::string> header;
		std::string line;

		header.reserve(CANVAS_HEADER_LINES);

		for (auto i = 0; i < CANVAS_HEADER_LINES; ++i)
		{
			std::getline(ifs, line);
			header.push_back(line);
		}

		return header;
	}

	void Canvas::GetPPMHeaderInfo(std::istream& ifs)
	{
		std::vector<std::string> headerLines = ReadPPMHeaderRaw(ifs);
		const std::string& sizeLine = headerLines[1];
		auto indexSpace = sizeLine.find(" ");

		auto width_ll = atoll(sizeLine.substr(0, indexSpace).c_str());
		auto height_ll = atoll(sizeLine.substr(indexSpace + 1, sizeLine.size() - indexSpace).c_str());

		width = static_cast<size_t>(width_ll);
		height = static_cast<size_t>(height_ll);
	}

	void Canvas::GetPPMBodyData(std::istream& ifs)
	{

	}

	void Canvas::WritePPMFile()
	{
		std::ofstream ofs(filename.c_str(), std::ofstream::out);

		WritePPMHeader(ofs);
		WritePPMBody(ofs);
	}

	void Canvas::ReadPPMFile()
	{
		std::ifstream ifs(filename.c_str(), std::ifstream::in);

		GetPPMHeaderInfo(ifs);
		GetPPMBodyData(ifs);
	}

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

		TEST_METHOD(WritingPPMFileHeader)
		{
			size_t width = 10;
			size_t height = 5;

			Canvas c{ width, height };

			std::stringstream stream;
			c.WritePPMHeader(stream);			
			std::vector<std::string> headerLines = c.ReadPPMHeaderRaw(stream);

			Assert::AreEqual(headerLines.at(0).c_str(), "P3");
			Assert::AreEqual(headerLines.at(1).c_str(), "10 5");
			Assert::AreEqual(headerLines.at(2).c_str(), "255");
		}

		TEST_METHOD(GettingPPMHeaderInfo)
		{
			size_t width = 10;
			size_t height = 5;

			Canvas c{ width, height };
			Canvas readerCanvas{ 0, 0 };

			std::stringstream stream;
			c.WritePPMHeader(stream);
			
			Assert::AreEqual(readerCanvas.width, size_t{ 0 });
			Assert::AreEqual(readerCanvas.height, size_t{ 0 });

			readerCanvas.GetPPMHeaderInfo(stream);

			Assert::AreEqual(readerCanvas.width, width);
			Assert::AreEqual(readerCanvas.height, height);
		}
	};
}
#endif
#pragma endregion