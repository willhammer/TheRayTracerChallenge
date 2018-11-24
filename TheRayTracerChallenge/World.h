#pragma once
#include "Serialization.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <memory>

#define WORLD_OBJECT_BODY											\
private:															\
	static Serialization::ClassID classID;							\
	const Serialization::ObjectID objectID;							\
public:																\
	const size_t GetClassID() const { return classID.GetID(); }		\
	const size_t GetObjectID() const { return objectID.GetID(); }	\


#define WORLD_OBJECT_CLASS_INIT(className) Serialization::ClassID className::classID = Serialization::ClassID()

namespace Gameplay
{
	class WorldObject;
}
using ClassIDNumber = size_t;
using ObjectIDNumber = size_t;


using ObjectMap = std::map<ObjectIDNumber, std::unique_ptr<Gameplay::WorldObject>>;
using ObjectMapPtr = std::unique_ptr<ObjectMap>;
using ClassMap = std::map<ClassIDNumber, ObjectMapPtr>;
using ObjectPtr = std::unique_ptr<Gameplay::WorldObject>;

namespace Gameplay
{
	class Component
	{
		
	};

	class WorldObject
	{
		WORLD_OBJECT_BODY

	private:
		std::set<Component> components;
	};

	WORLD_OBJECT_CLASS_INIT(WorldObject);

	class World
	{
		#define Error_ObjectMapNotFound "Object Map not found!"
		#define Error_ObjectNotFound "Object not found!"

	private:
		ClassMap worldObjects;

	private:
		void AddWorldObject(WorldObject* object, size_t classID, size_t objectID)
		{	
			if (worldObjects.find(object->GetClassID()) == worldObjects.cend())
			{
				worldObjects.emplace(std::make_pair(classID, ObjectMapPtr{ new ObjectMap() }));
			}
			try
			{
				const ObjectMapPtr& worldObjectOfType = GetAllObjectsOfType(classID);
				worldObjectOfType->emplace
				(
					std::make_pair(objectID, ObjectPtr{ object })
				);
			}			
			catch (std::exception* e)
			{

			}
		}

	public:

		template<typename T, typename = std::enable_if_t<std::is_base_of_v<WorldObject, T>>>
		static void AddWorldObject(World* world, T* object)
		{
			const size_t classID = object->GetClassID();
			const size_t objectID = object->GetObjectID();			
			world->AddWorldObject(object, classID, objectID);
		}

		const ObjectMapPtr& GetAllObjectsOfType(size_t classID) const
		{
			if (worldObjects.find(classID) == worldObjects.cend())
				throw std::exception(Error_ObjectMapNotFound);

			return worldObjects.at(classID);
		}

		size_t GetNumObjectsOfType(size_t classID)
		{
			const auto& worldObjectOfType = GetAllObjectsOfType(classID);
			return worldObjectOfType->size();
		}

		size_t GetNumObjects()
		{
			size_t count = 0;

			for (auto& worldObjectsOfType : worldObjects)
			{
				if(!worldObjectsOfType.second->empty())
					count += worldObjectsOfType.second->size();
			}

			return count;
		}

		const std::unique_ptr<WorldObject>& GetObject(size_t classID, size_t objectID) const
		{
			const auto& worldObjectOfType = GetAllObjectsOfType(classID);
			if (worldObjectOfType->empty())
				throw std::exception(Error_ObjectNotFound);

			return worldObjectOfType->at(objectID);
		}
	};
}