// Inclusion des headers de la SDL

#include "SDLpp.hpp"
#include "SDLppTexture.hpp"
#include "SDLppWindow.hpp"
#include "SDLppFont.hpp"
#include "SDLppTTF.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

int main(int argc, char *argv[])
{
	try
	{
		SDLpp sdl;
		SDLppTTF ttfInit;
		SDLppFont font("resources/coolvetica.ttf", 48);

		SDLppWindow window("Ma super fenêtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
		SDLppRenderer renderer = window.CreateRenderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		SDLppSurface textSurface = font.RenderUTF8Blended("Bonjour l'IIm !");
		SDLppTexture textTexture = SDLppTexture::FromSurface(renderer, textSurface);

		SDLppTexture circleTexture = SDLppTexture::FromFile(renderer, "resources/circle.png");

		SDL_Rect textRect = textTexture.GetRect();
		SDL_Rect circleRect = circleTexture.GetRect();
		circleRect.x = 200;
		circleRect.y = 200;

		bool running = true;
		while (running)
		{
			SDL_Event event;
			while (sdl.PollEvent(event))
			{
				switch (event.type)
				{
					case SDL_QUIT:
						running = false;
						break;

					default:
						break;
				}
			}

			renderer.SetDrawColor(0, 0, 0);
			renderer.Clear();
			renderer.Copy(textTexture, textRect);
			renderer.Copy(circleTexture, circleRect);
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

/*
Ancienne version (sans les wrapper)

int main(int argc, char *argv[])
{
	// La fonction SDL_Init initialise certains sous-systèmes de la bibliothèque
	// Elle prend en paramètre des flags indiquants ce qu'il faut initialiser (dans notre cas SDL_INIT_VIDEO sera suffisant)
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// La dernière cause d'erreur peut être retrouvée via SDL_GetError()
		std::cerr << "failed to initialize SDL: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Initialisation de la bibliothèque SDL_ttf, comme pour SDL_Init elle retourne une valeur négative en cas d'erreur
	if (TTF_Init() < 0)
	{
		// En cas d'erreur de SDL_ttf, on peut récupérer la dernière erreur avec la fonction TTF_GetError()
		std::cerr << "failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// On charge une police d'écriture avec la fonction TTF_OpenFont prenant le chemin et la taille de police désirée
	// en cas d'erreur, la fonction renvoie un pointeur nul
	TTF_Font* font = TTF_OpenFont("resources/coolvetica.ttf", 48);
	if (!font)
	{
		std::cerr << "failed to open coolvetica.ttf: " << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Pour charger une image vers un tableau de pixel (SDL_Surface), on peut utiliser IMG_Load, prenant le chemin vers la ressource
	SDL_Surface* circleSurface = IMG_Load("resources/circle.png");
	if (!circleSurface)
	{
		// En cas d'erreur de SDL_image, on peut récupérer la dernière erreur avec la fonction IMG_GetError()
		std::cerr << "failed to load circle.png: " << IMG_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// SDL_CreateWindow comme son nom l'indique créé une fenêtre, les paramètres attendus sont :
	// - Le titre de la fenêtre
	// - sa position X / Y (constantes SDL_WINDOWPOS_CENTERED et SDL_WINDOWPOS_UNDEFINED acceptées)
	// - sa taille Width/Height
	// - Des paramtères de créations (0 ne spécifiant aucun flag)
	SDL_Window* window = SDL_CreateWindow("Ma super fenêtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
	if (!window)
	{
		std::cerr << "failed to initialize window: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Pour faire un rendu vers une fenêtre, on peut créer un Renderer avec SDL_CreateRenderer
	// Il demande la fenêtre, le type de renderer (-1 permet à la SDL de décider) et des flags
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		std::cerr << "failed to initialize renderer: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// On va ensuite rendre notre texte vers une surface
	// Il existe plusieurs façons de faire le rendu de notre texte, la fonction que nous privilégierons
	// est TTF_RenderUTF8_Blended (prenant la police, le texte et la couleur désirée).
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, "Bonjour l'IIM !", color);
	if (!textSurface)
	{
		std::cerr << "failed to render text: " << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture)
	{
		std::cerr << "failed to create texture from text: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Nous n'avons plus besoin de la surface du texte à partir d'ici, nous la libérons
	SDL_FreeSurface(textSurface);

	SDL_Texture* circleTexture = SDL_CreateTextureFromSurface(renderer, circleSurface);
	if (!circleTexture)
	{
		std::cerr << "failed to create texture from circle: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Nous n'avons plus besoin de la surface du cercle à partir d'ici, nous la libérons
	SDL_FreeSurface(circleSurface);

	// Pour faire le rendu de nos textures sans distorsion, nous récupérons sa taille

	int width;
	int height;

	// La fonction SDL_QueryTexture permet de récupérer certaines informations sur une texture,
	// ici nous récupérons sa taille.

	SDL_QueryTexture(textTexture, nullptr, nullptr, &width, &height);

	SDL_Rect textRect = { 0, 0, width, height };

	SDL_QueryTexture(circleTexture, nullptr, nullptr, &width, &height);

	SDL_Rect circleRect = { 300, 200, width, height };

	SDL_Rect srcRect = { 0, 0, 50, 50 };

	// Nous entrons ensuite dans la boucle principale, celle dans laquelle notre application va passer le plus clair de son temps
	bool running = true;
	while (running)
	{
		// À chaque tour de boucle (frame), nous appelons la fonction SDL_PollEvent.
		// Il est important d'appeler cette fonction régulièrement car elle fait tourner l'état interne
		// de la bibliothèque.
		// Elle nous renvoie 1 si un événement est en attente et 0 si plus aucun événement n'est en attente,
		// la façon classique d'utiliser les événements avec la SDL est avec une boucle while jusqu'à ce qu'il n'y ait
		// plus d'événement en attente.

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// On inspecte le type d'événement pour savoir à quoi nous avons affaire
			switch (event.type)
			{
				// SDL_QUIT est généré lorsqu'un utilisateur décide de fermer la dernière fenêtre
				// de l'application (ou demande explicitement la fermeture du programme selon les systèmes)
				case SDL_QUIT:
					running = false;
					break;

				default:
					break;
			}
		}

		// Après avoir traité les événements, nous passons au rendu

		// Nous configurons la couleur de la prochaine opération de dessin pour le clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		// Nous vidons l'écran (en le remplissant de la couleur précédemment choisie)
		SDL_RenderClear(renderer);
		// On copie notre texture sur la fenêtre (blit)
		// Les deux derniers paramètres indiquent le rectangle source et le rectangle de destination (nullptr indique tout la zone)
		SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
		SDL_RenderCopy(renderer, circleTexture, nullptr, &circleRect);
		// On présente le rendu (affichage du rendu vers la fenêtre)
		SDL_RenderPresent(renderer);
	}

	// On libère toutes les ressources avant de quitter l'application
	SDL_DestroyTexture(circleTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
*/