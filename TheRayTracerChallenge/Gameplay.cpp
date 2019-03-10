#include "stdafx.h"
#include "Gameplay.h"
#include "Graphics.h"

#include "Math_Tuple.h"

#ifdef _MSC_VER
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#endif

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
	namespace G = Graphics;

	TEST_CLASS(GameplayTest)
	{
	public:
		TEST_METHOD(Tick)
		{
			Environment env;
			Projectile projectile;
			
			Vector4f setVelocity;			
			projectile.Tick(env, setVelocity);
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::X), 0.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::Y), 0.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::Z), 0.0f));
			
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::X), 0.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::Y), 0.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::Z), 0.0f));
			
			setVelocity = H::MakeVector(3.0f, 4.0f, 5.0f);
			projectile.Tick(env, setVelocity);
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::X), 0.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::Y), 0.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::Z), 0.0f));

			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::X), 3.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::Y), 4.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::Z), 5.0f));

			projectile.Tick(env, setVelocity);
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::X), 3.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::Y), 4.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.position, C::Z), 5.0f));

			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::X), 3.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::Y), 4.0f));
			Assert::IsTrue(M::Equals(H::Get(projectile.velocity, C::Z), 5.0f));
		}


		TEST_METHOD(PlotProjectileOnCanvas)
		{
			Environment env;
			Projectile projectile;

			Color4f projectileColor = H::MakeColor(1.0f, 0.0f, 0.0f, 0.5f);
			Vector4f setVelocity = H::MakeVector(10.0f, 15.0f, 0.0f);

			env.gravity = H::MakeVector(0.0f, -1.0f, 0.0f);
			env.wind = H::MakeVector(0.0f, 0.0f, 0.0f);

			std::vector<Point4f> projectilePositions;
			projectilePositions.reserve(11);

			projectile.Tick(env, setVelocity);
			projectilePositions.push_back(projectile.position);

			while (H::Get(projectile.position, C::Y) >= 0)
			{
				projectile.Tick(env, projectile.velocity);
				projectilePositions.push_back(projectile.position);
			}
			
			float width = 16 * 20;
			float height = 9 * 20;

			G::Canvas canvas((size_t)width, (size_t)height);
			for (const Point4f& position : projectilePositions)
			{
				auto posX = H::Get(position, C::X);
				auto posY = H::Get(position, C::Y);
				if (posX < 0.0f) posX = 0.0f;
				if (posX > width - 1) posX = width - 1;

				if (posY < 0.0f) posY = 0.0f;
				if (posY > height - 1) posY = height - 1;
				
				canvas.SetAt((size_t)posY, (size_t)posX, projectileColor);
			}

			canvas.SetFilename("projectileplot.ppm");
			canvas.WritePPMFile();
		}
	};
}
#endif
#pragma endregion