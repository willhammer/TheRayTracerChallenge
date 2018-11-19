#pragma once
#include "Serialization.h"

#define WORLD_OBJECT_BODY									\
private:															\
	static Serialization::ClassID classID;							\
	const Serialization::ObjectID objectID;							\
public:																\
	const size_t GetClassID() const { return classID.GetID(); }		\
	const size_t GetObjectID() const { return objectID.GetID(); }	\


#define WORLD_OBJECT_CLASS_INIT(className) ClassID className::classID = ClassID()