#include "sdlcpp/SDLpp.hpp"
#include "sdlcpp/SDLppFont.hpp"
#include "sdlcpp/SDLppRenderer.hpp"
#include "sdlcpp/SDLppSurface.hpp"
#include "sdlcpp/SDLppTexture.hpp"
#include "sdlcpp/SDLppTTF.hpp"
#include "sdlcpp/SDLppWindow.hpp"
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

struct Drawable
{
	int width;
	int height;
	std::shared_ptr<SDLppTexture> texture;
};

struct NoGravity {};

struct Input
{
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
};

void PlayerControllerSystem(entt::registry& registry);
void InputSystem(entt::registry& registry);
void GravitySystem(entt::registry& registry, float elapsedTime);
void RenderSystem(entt::registry& registry, SDLppRenderer& renderer);
void VelocitySystem(entt::registry& registry, float elapsedTime);

int main()
{
	try
	{
		SDLpp sdl;

		SDLppWindow window("Ma super fenêtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		SDLppRenderer renderer = window.CreateRenderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		std::shared_ptr<SDLppTexture> circleTexture = SDLppTexture::FromFile(renderer, "resources/circle.png");
		SDL_Rect circleRect = circleTexture->GetRect();
		circleRect.x = 200;
		circleRect.y = 200;

		entt::registry registry;

		Uint64 lastTime = sdl.GetPerformanceCounter();
		Uint64 freq = sdl.GetPerformanceFrequency();


		entt::entity player = registry.create();
		{
			auto& entityPos = registry.emplace<Position>(player);
			entityPos.x = 200.f;
			entityPos.y = 200.f;

			auto& entityDrawable = registry.emplace<Drawable>(player);
			entityDrawable.width = 640.f / 5.f;
			entityDrawable.height = 427.f / 5.f;
			entityDrawable.texture = SDLppTexture::FromFile(renderer, "resources/player.png");

			auto& entityVelocity = registry.emplace<Velocity>(player);
			entityVelocity.x = 0.f;
			entityVelocity.y = 0.f;

			registry.emplace<NoGravity>(player);
			input = registry.emplace<Input>(player);
		}

		bool running = true;
		while (running)
		{
			Uint64 now = sdl.GetPerformanceCounter();
			float elapsedTime = static_cast<float>(now - lastTime) / static_cast<float>(freq);
			lastTime = now;

			SDL_Event event;
			while (sdl.PollEvent(event))
			{
				switch (event.type)
				{
					case SDL_QUIT:
						running = false;
						break;

					case SDL_MOUSEBUTTONDOWN:
					{
						if (event.button.button == SDL_BUTTON_LEFT)
						{
							entt::entity entity = registry.create();
							auto& entityPos = registry.emplace<Position>(entity);
							entityPos.x = event.button.x;
							entityPos.y = event.button.y;

							auto& entityDrawable = registry.emplace<Drawable>(entity);
							entityDrawable.width = rand() % 100 + 100;
							entityDrawable.height = rand() % 100 + 100;
							entityDrawable.texture = circleTexture;

							auto& entityVelocity = registry.emplace<Velocity>(entity);
							entityVelocity.x = 0.f;
							entityVelocity.y = 0.f;
						}
						break;
					}

					default:
						break;
				}
			}

			renderer.SetDrawColor(0, 0, 0);
			renderer.Clear();
			InputSystem(registry);
			PlayerControllerSystem(registry);
			GravitySystem(registry, elapsedTime);
			VelocitySystem(registry, elapsedTime);
			RenderSystem(registry, renderer);
			renderer.Present();
		}

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

void PlayerControllerSystem(entt::registry& registry)
{
	auto view = registry.view<Input, Velocity>();
	view.each([](const Input& input, Velocity& velocity)
	{
		velocity.x = 0.f;
		velocity.y = 0.f;

		if (input.up)
			velocity.y += -500.f;

		if (input.down)
			velocity.y += 500.f;

		if (input.left)
			velocity.x += -500.f;

		if (input.right)
			velocity.x += 500.f;
	});
}

void InputSystem(entt::registry& registry)
{
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	auto view = registry.view<Input>();
	for (entt::entity entity : view)
	{
		auto& input = view.get<Input>(entity);
		input.down = state[SDL_SCANCODE_DOWN];
		input.left = state[SDL_SCANCODE_LEFT];
		input.right = state[SDL_SCANCODE_RIGHT];
		input.up = state[SDL_SCANCODE_UP];
	}
}

void GravitySystem(entt::registry& registry, float elapsedTime)
{
	const float GravityConstant = 981.f;

	auto view = registry.view<Velocity>(entt::exclude<NoGravity>);
	for (entt::entity entity : view)
	{
		auto& entityVel = view.get<Velocity>(entity);

		entityVel.y += GravityConstant * elapsedTime;
	}
}

void RenderSystem(entt::registry& registry, SDLppRenderer& renderer)
{
	auto view = registry.view<Position, Drawable>();
	for (entt::entity entity : view)
	{
		auto& entityPos = view.get<Position>(entity);
		auto& entityDrawable = view.get<Drawable>(entity);

		SDL_Rect rect;
		rect.x = static_cast<int>(entityPos.x);
		rect.y = static_cast<int>(entityPos.y);
		rect.w = entityDrawable.width;
		rect.h = entityDrawable.height;

		renderer.Copy(*entityDrawable.texture, rect);
	}
}

void VelocitySystem(entt::registry& registry, float elapsedTime)
{
	auto view = registry.view<Position, Velocity>();
	for (entt::entity entity : view)
	{
		auto& entityPos = view.get<Position>(entity);
		auto& entityVel = view.get<Velocity>(entity);

		entityPos.x += entityVel.x * elapsedTime;
		entityPos.y += entityVel.y * elapsedTime;
	}
}
