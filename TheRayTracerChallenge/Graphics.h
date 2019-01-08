#pragma once

#include <type_traits>
#include <limits>
#include <variant>
#include <vector>
#include <fstream>
#include <sstream>
#include "Math.h"


namespace M = Math;
using Color4f = M::Color4<float>;

#define CANVAS_HEADER_LINES 3

namespace Graphics
{

	class Canvas
	{
		friend class GraphicsTest;

	private:
		size_t width;
		size_t height;

		std::string filename;
		std::vector<Color4f> contents;

	private:
		void WritePPMHeader(std::ostream& ofs);
		void WritePPMBody(std::ostream& ofs);

		std::vector<std::string> ReadPPMHeaderRaw(std::istream& ifs);
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

