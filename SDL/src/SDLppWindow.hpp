#pragma once

#include "SDLppRenderer.hpp"
#include <SDL2/SDL.h>
#include <string>

class SDLppWindow
{
public:
	SDLppWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags = 0);
	SDLppWindow(const SDLppWindow&) = delete;
	SDLppWindow(SDLppWindow&& font);
	~SDLppWindow();

	SDLppRenderer CreateRenderer(Uint32 flags);
	SDLppRenderer CreateRenderer(int index, Uint32 flags);

	SDLppWindow& operator=(const SDLppWindow&) = delete;
	SDLppWindow& operator=(SDLppWindow&& font);

private:
	SDL_Window* m_window;
};