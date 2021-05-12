#include "SDLpp.hpp"
#include <stdexcept>

SDLpp::SDLpp()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(std::string("failed to initialize SDL: ") + SDL_GetError());
}

SDLpp::~SDLpp()
{
	SDL_Quit();
}

bool SDLpp::PollEvent(SDL_Event& event)
{
	return SDL_PollEvent(&event);
}
