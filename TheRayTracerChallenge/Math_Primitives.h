#pragma once


#include "stdafx.h"
#include "Math_Common.h"
#include "Math_Matrix.h"
#include "Math_Tuple.h"

namespace Math
{
	class Object
	{
	private:
		static size_t objectIdCounter;
		size_t objectId;
		
	public:
		Object()
		{
			objectId = ++objectIdCounter;
		}

		size_t GetObjectId() { return objectId; }

	};
	
	size_t Object::objectIdCounter = 0;

	template<typename T>
	class Sphere : public Object
	{
	private:
		T radius;
		Vector4<T> position;

	public:

		Sphere() : radius{0}, position{H::MakeVector(T(0), T(0), T(0))}
		{

		}

		Sphere(T setRadius, Vector4<T> setPosition) :
			radius{setRadius},
			position{setPosition}
		{
			
		}
	};
}
