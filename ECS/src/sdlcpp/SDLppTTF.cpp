#include "SDLppTTF.hpp"
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

SDLppTTF::SDLppTTF()
{
	if (TTF_Init() < 0)
		throw std::runtime_error(std::string("failed to initialize SDL_ttf: ") + TTF_GetError());
}

SDLppTTF::~SDLppTTF()
{
	TTF_Quit();
}
