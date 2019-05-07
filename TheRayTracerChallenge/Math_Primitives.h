#pragma once


#include "stdafx.h"
#include "Math_Common.h"
#include "Math_Matrix.h"
#include "Math_Tuple.h"
#include "Math_Transform.h"
#include <unordered_map>
#include <unordered_set>

namespace Math
{
    template<typename T>
    class IRenderData
    {
    protected:
        Transform<T> transform;

    public:
        virtual Vector4<T> GetNormalAtPoint(const Point4<T>& point) = 0;
        Transform<T> GetTransform() { return transform; }
        void SetTransform(const Transform<T>& setTransform) { transform = setTransform; }
    };

	class Object
	{
	private:
		static size_t objectIdCounter;
		static std::unordered_map<size_t, Object*> objectMap;
		size_t objectId;
		
	public:
		static void ResetObjectMap()
		{
			objectMap.clear();
			objectIdCounter = 0;
		}

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
	
	template<typename T>
	class Sphere : public Object, public IRenderData<T>
	{
	private:
		T radius;
		Point4<T> position;

	public:
		Sphere() : radius{0}, position{H::MakePoint(T(0), T(0), T(0))}
        {
            SetTransform(Transform<T>::Identity());
        }

		Sphere(T setRadius, Point4<T> setPosition) : radius{setRadius}, position{setPosition}{ }

        T GetRadius() { return radius; }
        Point4<T> GetPosition() { return position; }

        void SetRadius(const T setRadius) { radius = setRadius; }
        void SetPosition(const Point4<T>& setPosition) { position = setPosition; }

        Vector4<T> GetNormalAtPoint(const Point4<T>& point) override;
	};
}
