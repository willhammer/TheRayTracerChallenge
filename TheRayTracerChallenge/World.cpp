#include "stdafx.h"
#include "World.h"

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

	TEST_CLASS(SerializationTest)
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
#endif