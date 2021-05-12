#include "SDLppSurface.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>

SDLppSurface::SDLppSurface(SDL_Surface* surface) :
m_surface(surface)
{
}

SDLppSurface::SDLppSurface(SDLppSurface&& surface)
{
	m_surface = surface.m_surface;
	surface.m_surface = nullptr;
}

SDLppSurface::~SDLppSurface()
{
	if (m_surface)
		SDL_FreeSurface(m_surface);
}

SDL_Surface* SDLppSurface::GetHandle() const
{
	return m_surface;
}

SDLppSurface& SDLppSurface::operator=(SDLppSurface&& surface)
{
	if (m_surface)
		SDL_FreeSurface(m_surface);

	m_surface = surface.m_surface;
	surface.m_surface = nullptr;

	return *this;
}

SDLppSurface SDLppSurface::FromFile(const std::string& filepath)
{
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	if (!surface)
		throw std::runtime_error("failed to load " + filepath + ": " + IMG_GetError());

	return SDLppSurface(surface);
}
