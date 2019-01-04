#include "stdafx.h"
#include "Gameplay.h"


#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

using namespace Math;

namespace Gameplay
{
	void Projectile::Tick(const Environment& env, const Vector4f setVelocity)
	{
		this->position = this->position + this->velocity;
		this->velocity = setVelocity + env.gravity + env.wind;
	}
};


#pragma region Math Tests
#ifdef _MSC_VER
namespace Gameplay
{	
	namespace M = Math;
	using H = Math::Helpers;
	using C = Math::Helpers::Coordinate;
	
	TEST_CLASS(GameplayTest)
	{
	public:
		TEST_METHOD(Tick)
		{
			Environment env;
			Projectile projectile;
			
			Vector4f setVelocity;			
			projectile.Tick(env, setVelocity);
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::X), 0.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::Y), 0.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::Z), 0.0f));
			
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::X), 0.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::Y), 0.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::Z), 0.0f));
			
			setVelocity = H::MakeVector(3.0f, 4.0f, 5.0f);
			projectile.Tick(env, setVelocity);
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::X), 0.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::Y), 0.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::Z), 0.0f));

			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::X), 3.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::Y), 4.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::Z), 5.0f));

			projectile.Tick(env, setVelocity);
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::X), 3.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::Y), 4.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.position, C::Z), 5.0f));

			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::X), 3.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::Y), 4.0f));
			Assert::IsTrue(Math::Equals(H::Get(projectile.velocity, C::Z), 5.0f));
		}
	};
}
#endif
#pragma endregion