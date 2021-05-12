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
	float y = -10.f;
};

struct Drawable
{
	int width;
	int height;
	std::shared_ptr<SDLppTexture> texture;
};

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

		bool running = true;
		while (running)
		{
			Uint64 now = sdl.GetPerformanceCounter();
			float elapsedTime = static_cast<float>(now - lastTime) / freq;
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
							entityDrawable.width = 100;
							entityDrawable.height = 100;
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

void GravitySystem(entt::registry& registry, float elapsedTime)
{
	const float GravityConstant = 981.f;

	auto view = registry.view<Velocity>();
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
