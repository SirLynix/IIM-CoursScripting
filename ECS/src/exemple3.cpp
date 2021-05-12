#include <entt/entt.hpp>
#include <iostream>

struct Position
{
	float x = 0.f;
	float y = 0.f;
};

struct Velocity
{
	float x = 0.f;
	float y = 0.f;
};

int main()
{
	entt::registry registry;

	entt::entity maSuperEntity = registry.create();
	{
		auto& position = registry.emplace<Position>(maSuperEntity);
		position.x = 50.f;

		auto& velocity = registry.emplace<Velocity>(maSuperEntity);
		velocity.x = -10.f;

		registry.emplace<std::string>(maSuperEntity, "Alexis");
	}

	entt::entity maSuperEntity2 = registry.create();
	{
		registry.emplace<std::string>(maSuperEntity2, "Matilde");
		registry.emplace<Position>(maSuperEntity2);
	}

	for (int i = 0; i < 100; ++i)
	{
		auto velocityView = registry.view<Position, Velocity>();
		for (entt::entity entity : velocityView)
		{
			auto& pos = velocityView.get<Position>(entity);
			auto& vel = velocityView.get<Velocity>(entity);

			pos.x += vel.x;
			pos.y += vel.y;
		}

		auto listView = registry.view<Position, std::string>();
		for (entt::entity entity : listView)
		{
			auto& pos = listView.get<Position>(entity);
			auto& name = listView.get<std::string>(entity);

			std::cout << name << ": " << pos.x << ", " << pos.y << std::endl;
		}
	}
}
