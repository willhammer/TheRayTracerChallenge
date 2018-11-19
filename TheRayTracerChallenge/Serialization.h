#pragma once
#include <type_traits>
#include <limits>
#include <variant>

namespace Serialization
{
	enum class IDType : int
	{
		Class = 0,
		Object = 1
	};

	template<IDType Type>
	class Identification
	{
	protected:
		static size_t totalNumber;
		const size_t id;

	public:
		Identification() : id{ totalNumber }
		{
			++totalNumber;
		}

		const size_t GetID() const
		{
			return id;
		}
	};

	using ClassID = Identification<IDType::Class>;
	using ObjectID = Identification<IDType::Object>;

	size_t ClassID::totalNumber = 0;
	size_t ObjectID::totalNumber = 0;
}