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

const Uint8* SDLpp::GetKeyboardState(int* numkeys) const
{
	return SDL_GetKeyboardState(numkeys);
}

Uint64 SDLpp::GetPerformanceCounter() const
{
	return SDL_GetPerformanceCounter();
}

Uint64 SDLpp::GetPerformanceFrequency() const
{
	return SDL_GetPerformanceFrequency();
}

bool SDLpp::PollEvent(SDL_Event& event)
{
	return SDL_PollEvent(&event);
}
