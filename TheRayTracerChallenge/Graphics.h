#pragma once

#include <type_traits>
#include <limits>
#include <variant>
#include <vector>
#include "Math.h"


namespace M = Math;
using Color4f = M::Color4<float>;


namespace Graphics
{
	
	class Canvas
	{
	private:
		size_t width;
		size_t height;

		std::vector<Color4f> contents;

	public:
		Canvas(size_t setWidth, size_t setHeight) : width{ setWidth }, height{ setHeight } 
		{
			contents.resize(width * height);
		}
		
		Color4f GetAt(size_t line, size_t column) 
		{			
			//how do I properly assert on the size of line and column? 
			//I cannot statically check this, so I can't use enable_if, nor static_assert
			return contents.at(line * width + column);
		}

		void SetAt(size_t line, size_t column, Color4f value)
		{
			contents.at(line * width + column) = value;
		}
	};

};

