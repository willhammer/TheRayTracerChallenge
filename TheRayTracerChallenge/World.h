#pragma once
#include "Serialization.h"
#include <map>
#include <vector>
#include <set>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <chrono>
#include <functional>

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

using TimePoint = std::chrono::high_resolution_clock::time_point;

using PerObjectFunction = std::function<void(Gameplay::WorldObject*, const float)>;

namespace Gameplay
{
	class Component
	{
		
	};

	class WorldObject
	{
		WORLD_OBJECT_BODY

	public:
		virtual void Step(float deltaTime) { throw new std::exception("function execution at WorldObject level is not allowed!"); }

	private:
		std::set<Component> components;
	};

	WORLD_OBJECT_CLASS_INIT(WorldObject);

	class World
	{
		#define Error_ObjectMapNotFound "Object Map not found!"
		#define Error_ObjectNotFound "Object not found!"

		#define DEFAULT_PHYSICS_DELTA_TIME 0.0333333f
		#define DEFAULT_MAX_FRAME_TIME 0.25f
		#define DEFAULT_MIN_FRAME_TIME 0.005f

	private:
		bool isStarted;		
		bool isPaused;
		

		const float minFrameTime;
		const float maxFrameTime;
		const float physicsDeltaTime;

		float deltaTimeAccumulator;
		TimePoint currentTime;
		
	public:		
		World(float setPhysicsDeltaTime, float setMaxFrameTime, float setMinFrameTime) : 
			physicsDeltaTime{ setPhysicsDeltaTime },
			maxFrameTime { setMaxFrameTime },
			minFrameTime { setMinFrameTime}
		{

		}

		World() :World(DEFAULT_PHYSICS_DELTA_TIME, DEFAULT_MAX_FRAME_TIME, DEFAULT_MIN_FRAME_TIME)
		{

		}

		void Start();
		void Stop(bool forced);
		void Pause();

	private:
		void ForceStop();
		void Run();
		
		void RunPhysics(float physicsDeltaTime);
		void RunRender(float renderDeltaTime);

	private:
		ClassMap worldObjects;
	
	#pragma region METHODS: OBJECT MANAGEMENT
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
				printf("%s", e->what());
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
	#pragma endregion


	#pragma region METHODS: STEP LOGIC



	#pragma endregion

	};
}