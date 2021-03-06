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
void InputSystem(const SDLpp& sdl, entt::registry& registry);
void GravitySystem(entt::registry& registry, float elapsedTime);
void RenderSystem(entt::registry& registry, SDLppRenderer& renderer);
void VelocitySystem(entt::registry& registry, float elapsedTime);

int main()
{
	try
	{
		SDLpp sdl;

		SDLppWindow window("Ma super fenétre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		SDLppRenderer renderer = window.CreateRenderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		// On charge la texture des cercles que nous allons afficher dans un std::shared_ptr
		// afin de pouvoir la partager ensuite entre nos entités cercles
		std::shared_ptr<SDLppTexture> circleTexture = SDLppTexture::FromFile(renderer, "resources/circle.png");

		entt::registry registry;

		// On créé une entité joueur (présente dés le début) avec des composants particuliers
		entt::entity player = registry.create();
		{
			// Une position de départ
			auto& entityPos = registry.emplace<Position>(player);
			entityPos.x = 200.f;
			entityPos.y = 200.f;

			// Une façon de l'afficher
			auto& entityDrawable = registry.emplace<Drawable>(player);
			entityDrawable.width = 640.f / 5.f;
			entityDrawable.height = 427.f / 5.f;
			entityDrawable.texture = SDLppTexture::FromFile(renderer, "resources/player.png");

			// Une vélocité
			auto& entityVelocity = registry.emplace<Velocity>(player);
			entityVelocity.x = 0.f;
			entityVelocity.y = 0.f;

			// Nous ne voulons pas qu'il soit soumis à la gravité
			registry.emplace<NoGravity>(player);

			// Nous voulons pouvoir le contréler
			registry.emplace<Input>(player);
		}

		// SDL_GetPerformanceCounter nous renvoie un nombre qui ne fait que croitre avec le temps
		Uint64 lastTime = sdl.GetPerformanceCounter();
		// SDL_GetPerformanceFrequency nous renvoie l'incrément que prendra le nombre en une seconde
		Uint64 freq = sdl.GetPerformanceFrequency();

		bool running = true;
		while (running)
		{
			// On se sert du performance counter pour récupérer le temps écoulé depuis la derniére
			// itération de la boucle (autrement dit, le temps pris par la derniére boucle)
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

					// Lorsqu'un bouton de la souris est déclenché
					case SDL_MOUSEBUTTONDOWN:
					{
						// et que ce bouton est le bouton gauche de la souris
						if (event.button.button == SDL_BUTTON_LEFT)
						{
							// On créé une entité, possédant Position, Velocity et Drawable pour représenter un cercle
							entt::entity entity = registry.create();
							auto& entityPos = registry.emplace<Position>(entity);
							entityPos.x = event.button.x;
							entityPos.y = event.button.y;

							// Note, en C++ moderne on utiliserait plutôt le header <random> à la place de std::rand
							// à cause de son manque de précisions et de garanties sur son fonctionnement

							auto& entityDrawable = registry.emplace<Drawable>(entity);
							entityDrawable.width = rand() % 100 + 100;
							entityDrawable.height = rand() % 100 + 100;
							entityDrawable.texture = circleTexture;

							// On veut que nos cercles puissent se déplacer et ait une vélocité initiale aléatoire
							auto& velocity = registry.emplace<Velocity>(entity);
							velocity.x = rand() % 1000 - 500;
							velocity.y = -(rand() % 1000);
						}
						break;
					}

					default:
						break;
				}
			}

			// Mise à jour de l'état des entités dans un ordre particulier

			// Le réle de l'input system est de récupérer l'état du clavier 
			// et de l'appliquer à l'input component des entités en ayant un (le joueur)
			InputSystem(sdl, registry);

			// Le Player Controller system applique les inputs à sa vélocité
			PlayerControllerSystem(registry);

			// Le Gravity system accroit la vitesse (vers le bas) d'une entité au fil du temps
			GravitySystem(registry, elapsedTime);

			// Le velocity system répercute la vélocité sur la position
			VelocitySystem(registry, elapsedTime);

			// Rendu de la scéne (on vide l'écran, on affiche les entités avec un systéme et on le présente)
			renderer.SetDrawColor(0, 0, 0);
			renderer.Clear();

			// Le render system affiche ensuite chaque entité disposant d'une position et d'un Drawable
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

void InputSystem(const SDLpp& sdl, entt::registry& registry)
{
	const Uint8* state = sdl.GetKeyboardState();

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

	// Nous ne voulons que les entités ayant une vélocité (et n'ayant pas de composant NoGravity)
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
