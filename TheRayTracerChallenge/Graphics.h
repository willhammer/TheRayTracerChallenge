#pragma once

#include <type_traits>
#include <limits>
#include <variant>
#include <vector>
#include <fstream>
#include <sstream>


namespace Math
{
	template<typename T>
	class Color4;

	template<typename T>
	class Tuple4;
}

using Color4f = Math::Color4<float>;
using Tuple4f = Math::Tuple4<float>;


namespace Graphics
{
	class Canvas
	{
		friend class GraphicsTest;

	private:
		size_t width;
		size_t height;

		int maxValue;

		std::string filename;
		std::vector<Color4f> contents;

	private:
		void WritePPMHeader(std::ostream& ofs);
		void WritePPMBody(std::ostream& ofs);

		std::vector<std::string> ReadPPMHeaderRaw(std::istream& ifs);
		std::vector<std::string> ReadPPMBodyRaw(std::istream& ifs);
		void GetPPMHeaderInfo(std::istream& ifs);
		void GetPPMBodyData(std::istream& ifs);

	public:
		Canvas(size_t setWidth, size_t setHeight);
		Color4f GetAt(size_t line, size_t column);
		void SetAt(size_t line, size_t column, Color4f value);
		void SetFilename(std::string setFilename);

		void WritePPMFile();
		void ReadPPMFile();
	};
}

