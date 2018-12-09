#include "stdafx.h"
#include "World.h"
#include <thread>


using namespace std::chrono;

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif


#pragma region Serialization Tests
#ifdef _MSC_VER
namespace Serialization
{
	class C1
	{
		WORLD_OBJECT_BODY
	};

	class C2
	{
		WORLD_OBJECT_BODY
	};

	WORLD_OBJECT_CLASS_INIT(C1);
	WORLD_OBJECT_CLASS_INIT(C2);

	TEST_CLASS(ClassIDAndObjectIDTest)
	{
	public:
		TEST_METHOD(ClassIDIncrementsProperly)
		{
			ClassID id1;
			ClassID id2;

			Assert::AreEqual(id2.GetID() - id1.GetID(), (size_t)1);
		}

		TEST_METHOD(ObjectIDIncrementsProperly)
		{
			ObjectID id1;
			ObjectID id2;

			Assert::AreEqual(id2.GetID() - id1.GetID(), (size_t)1);
		}

		TEST_METHOD(ObjectIDAndClassIDIncrementProperly)
		{
			ClassID id1;
			ClassID id2;

			Assert::AreEqual(id2.GetID() - id1.GetID(), (size_t)1);

			ObjectID id3;
			ObjectID id4;

			Assert::AreEqual(id4.GetID() - id3.GetID(), (size_t)1);
		}

		TEST_METHOD(SerializableObjectsIncrementProperly)
		{
			C1 obj11;
			C1 obj12;
			C1 obj13;

			C2 obj21;
			C2 obj22;
			C2 obj23;

			Assert::AreEqual(obj12.GetObjectID() - obj11.GetObjectID(), (size_t)1);
			Assert::AreEqual(obj13.GetObjectID() - obj11.GetObjectID(), (size_t)2);

			Assert::AreEqual(obj22.GetObjectID() - obj21.GetObjectID(), (size_t)1);
			Assert::AreEqual(obj23.GetObjectID() - obj21.GetObjectID(), (size_t)2);

			Assert::AreEqual(obj21.GetClassID() - obj11.GetClassID(), (size_t)1);
			Assert::AreEqual(obj22.GetClassID() - obj11.GetClassID(), (size_t)1);
		}
	};
}

namespace
{
	void RunPhysicsOnObject(Gameplay::WorldObject* object, const float deltaTime)
	{
		//get physics component and do stuff
	}

	void RunRenderOnObject(Gameplay::WorldObject* object, const float deltaTime)
	{
		//get mesh component and do stuff
	}

	void RunFunctionOnAllObjects(ClassMap& worldObjects, PerObjectFunction perObjectFunction, const float deltaTime)
	{
		for (auto& objectMapPair = worldObjects.begin(); objectMapPair != worldObjects.end(); ++objectMapPair)
		{
			ObjectMap* objectMap = objectMapPair->second.get();
			if (objectMap != nullptr)
			{
				for (auto& objectPair = objectMap->begin(); objectPair != objectMap->end(); ++objectPair)
				{
					Gameplay::WorldObject* object = objectPair->second.get();
					if (object != nullptr)
					{
						perObjectFunction(object, deltaTime);
					}
				}
			}
		}
	}
}

namespace Gameplay
{

	void World::RunPhysics(float physicsDeltaTime)
	{
		RunFunctionOnAllObjects(worldObjects, PerObjectFunction(RunPhysicsOnObject), physicsDeltaTime);
	}

	void World::RunRender(float renderDeltaTime)
	{
		RunFunctionOnAllObjects(worldObjects, PerObjectFunction(RunRenderOnObject), renderDeltaTime);
	}

	void World::Start()
	{
		if (isStarted == true && isPaused == true)
			isPaused = true;

		if (isStarted == false)
		{
			isStarted = true;			
			Run();
		}	
	}

	void World::ForceStop()
	{

	}

	void World::Stop(bool forced)
	{
		if(isStarted == true)
			isStarted = false;

		if (forced)
		{
			ForceStop();
		}
	}

	void World::Pause()
	{
		isPaused = true;
	}

	void World::Run()
	{
		currentTime = high_resolution_clock::now();
		deltaTimeAccumulator = 0.0f;

		while (isStarted == true)
		{
			TimePoint newTime = high_resolution_clock::now();
			duration<float> timeSpan = duration_cast<duration<float>>(newTime - currentTime);
			
			float frameTime = timeSpan.count();

			if (frameTime > maxFrameTime)
				frameTime = maxFrameTime;

			deltaTimeAccumulator += frameTime;

			while (deltaTimeAccumulator >= physicsDeltaTime)
			{
				RunPhysics(physicsDeltaTime);
				deltaTimeAccumulator -= physicsDeltaTime;
			}
			
			RunRender(frameTime);			
		}
	}

#pragma region WORLD TESTS

	class Parent1 : public WorldObject
	{
		WORLD_OBJECT_BODY
	};

	class Parent2 : public WorldObject
	{
		WORLD_OBJECT_BODY
	};

	class Child11 : public Parent1
	{
		WORLD_OBJECT_BODY
	};

	class Child12 : public Parent1
	{
		WORLD_OBJECT_BODY
	};

	class Child21 : public Parent2
	{
		WORLD_OBJECT_BODY
	};

	class Child22 : public Parent2
	{
		WORLD_OBJECT_BODY
	};

	WORLD_OBJECT_CLASS_INIT(Parent1);
	WORLD_OBJECT_CLASS_INIT(Parent2);
	WORLD_OBJECT_CLASS_INIT(Child11);
	WORLD_OBJECT_CLASS_INIT(Child12);
	WORLD_OBJECT_CLASS_INIT(Child21);
	WORLD_OBJECT_CLASS_INIT(Child22);

	TEST_CLASS(AccessingWorldObjectsFromWorldTest)
	{	

	public:
		TEST_METHOD(AddOneObjectInWorld)
		{
			World world;

			Child11* object1 = new Child11();
			World::AddWorldObject(&world, object1);
			
			Assert::AreEqual(world.GetNumObjects(), (size_t)1);
			Assert::AreEqual(world.GetNumObjectsOfType(object1->GetClassID()), (size_t)1);
		}

		TEST_METHOD(AddTwoObjectsInWorld)
		{
			World world;

			Child11* object1 = new Child11();
			Child21* object2 = new Child21();
			World::AddWorldObject(&world, object1);
			World::AddWorldObject(&world, object2);

			Assert::AreEqual(world.GetNumObjects(), (size_t)2);
			Assert::AreEqual(world.GetNumObjectsOfType(object1->GetClassID()), (size_t)1);
			Assert::AreEqual(world.GetNumObjectsOfType(object2->GetClassID()), (size_t)1);
		}

		TEST_METHOD(AddTwoObjectsAndTheirParentsInWorld)
		{
			World world;

			Child11* object1 = new Child11();
			Child21* object2 = new Child21();
			Parent1* object3 = new Parent1();
			Parent2* object4 = new Parent2();

			World::AddWorldObject(&world, object1);
			World::AddWorldObject(&world, object2);
			World::AddWorldObject(&world, object3);
			World::AddWorldObject(&world, object4);

			Assert::AreEqual(world.GetNumObjects(), (size_t)4);
			Assert::AreEqual(world.GetNumObjectsOfType(object1->GetClassID()), (size_t)1);
			Assert::AreEqual(world.GetNumObjectsOfType(object2->GetClassID()), (size_t)1);
			Assert::AreEqual(world.GetNumObjectsOfType(object3->GetClassID()), (size_t)1);
			Assert::AreEqual(world.GetNumObjectsOfType(object4->GetClassID()), (size_t)1);
		}
	};

#pragma endregion
}
#endif