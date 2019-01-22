#include "stdafx.h"
#include "Graphics.h"

#include <sstream>
#include <array>

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

namespace
{
	template<typename T>
	int GetIntValue(const int maxIntValue, T value)
	{
		static T zero = static_cast<T>(0);
		static T one = static_cast<T>(1);
		static T max = static_cast<T>(maxIntValue);

		if (value < zero)
			value = zero;
		
		if (value > one)
			value = one;

		return int(value * max);
	}

	template<typename T>
	std::array<int, 3> GetIntColor(const int maxIntValue, const M::Color4<T> color)
	{
		std::array<int, 3> colorInt;

		auto colorElem = H::Get(color, CI::R);
		colorInt.at(0) = GetIntValue<T>(maxIntValue, colorElem);

		colorElem = H::Get(color, CI::G);
		colorInt.at(1) = GetIntValue<T>(maxIntValue, colorElem);
		
		colorElem = H::Get(color, CI::B);
		colorInt.at(2) = GetIntValue<T>(maxIntValue, colorElem);

		return colorInt;
	}
	
	int GetIntPrintSize(const int value, const bool firstElement)
	{
		//adding the space that comes before the number
		if (value >= 100)
			return firstElement ? 3 : 4;

		if (value >= 10)
			return firstElement ? 2 : 3;

		return firstElement ? 1 : 2;
	}

	std::vector<std::string> ReadLineValues(std::string& lineString)
	{
		std::vector<std::string> values;		
		std::string value;
		std::stringstream ss;
		ss << lineString;

		while (std::getline(ss, value, ' '))
		{
			values.push_back(value);
		}

		return values;
	}

}

namespace Graphics
{


	Canvas::Canvas(size_t setWidth, size_t setHeight) : width{ setWidth }, height{ setHeight }
	{
		contents.resize(width * height);
		maxValue = 255;
	}

	Color4f Canvas::GetAt(size_t line, size_t column)
	{
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

		snprintf(buff, sizeof(buff), "%d", maxValue);
		ofs << buff << std::endl;
	}

	void Canvas::WritePPMBody(std::ostream& ofs)
	{
		const int maxCharactersPerLine = 70;
		
		int charactersOnCurrentLine = 0;

		for (size_t i = 0; i < contents.size(); ++i)
		{
			auto& colorT = contents.at(i);
			auto colorInt = GetIntColor(maxValue, colorT);

			int currentPrintSize = 0;

			for (int value : colorInt)
			{	
				bool firstElement = charactersOnCurrentLine == 0;
				currentPrintSize = GetIntPrintSize(value, firstElement);
								
				if (charactersOnCurrentLine + currentPrintSize > maxCharactersPerLine)
				{
					ofs << std::endl;
					charactersOnCurrentLine = 0;
				}
				
				firstElement = charactersOnCurrentLine == 0;
				charactersOnCurrentLine += currentPrintSize;
				firstElement ? ofs << value : ofs << " " << value;
			}
		}
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

	std::vector<std::string> Canvas::ReadPPMBodyRaw(std::istream& ifs)
	{
		std::vector<std::string> bodyLines;
		std::string line;

		while (std::getline(ifs, line))
		{
			bodyLines.push_back(line);
		}

		return bodyLines;
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
		std::string line;
		std::string currentNumberAsString;
		unsigned char countValues = 0;
		static const int maxCountValues = 3;
		size_t index = 0;
		
		std::vector<std::string> lines = ReadPPMBodyRaw(ifs);		
		M::Tuple4<float> colorContents(0.0f, 0.0f, 0.0f, 0.5f);
		
		for (auto& line : lines)
		{
			std::vector<std::string> values = ReadLineValues(line);
			for (std::string& valueString : values)
			{
				float value = std::stof(valueString.c_str());
				value = value / maxValue;
				if (value < 0.0f) value = 0.0f;
				if (value > 1.0f) value = 1.0f;

				H::Set(colorContents, CI(countValues), value);
				++countValues;

				if (countValues == maxCountValues)
				{
					countValues = 0;
					contents.at(index) = H::MakeColor<float>(colorContents);
					++index;
				}
			}
		}

		auto a = index;
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

		TEST_METHOD(WritingPPMFileBody_MaxLength)
		{
			size_t width = 10;
			size_t height = 5;

			size_t maxLineLength = 70;

			Canvas c{ width, height };
			c.SetAt(0, 0, H::MakeColor(Tuple4f{ 1.5f, 0.0f, 0.0f, 0.5f }));
			c.SetAt(2, 1, H::MakeColor(Tuple4f{ 0.0f, 0.5f, 0.0f, 0.5f }));
			c.SetAt(4, 2, H::MakeColor(Tuple4f{ -0.5f, 0.0f, 1.0f, 0.5f }));
			
			std::stringstream stream;
			c.WritePPMBody(stream);
			
			Canvas newCanvas{ width, height };
			
			std::vector<std::string> writtenLines = newCanvas.ReadPPMBodyRaw(stream);
			
			auto size = writtenLines.size();
			Assert::IsTrue(size == 5);

			for(const auto& line : writtenLines)
			{
				auto length = line.length();
				Assert::IsTrue(length <= maxLineLength);
			}
		}

		TEST_METHOD(ReadingPPMFileBody)
		{
			size_t width = 10;
			size_t height = 5;

			size_t maxLineLength = 70;

			Canvas c{ width, height };
			c.SetAt(0, 0, H::MakeColor(Tuple4f{ 1.5f, 0.0f, 0.0f, 0.5f }));
			c.SetAt(2, 1, H::MakeColor(Tuple4f{ 0.0f, 0.5f, 0.0f, 0.5f }));
			c.SetAt(4, 2, H::MakeColor(Tuple4f{ -0.5f, 0.0f, 1.0f, 0.5f }));

			std::stringstream stream;
			c.WritePPMBody(stream);
			
			Canvas newCanvas{ width, height };
			newCanvas.GetPPMBodyData(stream);
			
			auto& first = newCanvas.GetAt(0, 0);
			auto second = H::MakeColor(1.0f, 0.0f, 0.0f, 0.5f);
			Assert::IsTrue(first == second);

			first = newCanvas.GetAt(2, 1);
			second = H::MakeColor(0.0f, 0.5f, 0.0f, 0.5f);
			Assert::IsTrue(first == second);
			
			first = newCanvas.GetAt(4, 2);
			second = H::MakeColor(0.0f, 0.0f, 1.0f, 0.5f);
			Assert::IsTrue(first == second);
		}
	};
}
#endif
#pragma endregion