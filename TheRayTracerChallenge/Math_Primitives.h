#pragma once


#include "stdafx.h"
#include "Math_Common.h"
#include "Math_Matrix.h"
#include "Math_Tuple.h"
#include <unordered_map>
#include <unordered_set>

namespace Math
{
	class Object
	{
	private:
		static size_t objectIdCounter;
		static std::unordered_map<size_t, Object*> objectMap;

		size_t objectId;
		
	public:

		static size_t GetNumObjects() { return objectMap.size(); }
		static Object* GetObjectById(size_t id) { return objectMap.at(id); }

		Object()
		{
			objectId = ++objectIdCounter;
			objectMap.emplace(std::pair<size_t, Object*>(objectId, this));
		}

		~Object()
		{
			auto entryForThis = objectMap.find(this->GetObjectId());
			objectMap.erase(entryForThis);
		}

		size_t GetObjectId() { return objectId; }

	};
	
	size_t Object::objectIdCounter = 0;
	std::unordered_map<size_t, Object*> Object::objectMap = std::unordered_map<size_t, Object*>();

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
