#pragma once

#include <SDL2/SDL.h>
#include <string>

class SDLppSurface
{
public:
	SDLppSurface(SDL_Surface* surface);
	SDLppSurface(const SDLppSurface&) = delete;
	SDLppSurface(SDLppSurface&& surface);
	~SDLppSurface();

	SDL_Surface* GetHandle() const;

	SDLppSurface& operator=(const SDLppSurface&) = delete;
	SDLppSurface& operator=(SDLppSurface&& surface);

	static SDLppSurface FromFile(const std::string& filepath);

private:
	SDL_Surface* m_surface;
};