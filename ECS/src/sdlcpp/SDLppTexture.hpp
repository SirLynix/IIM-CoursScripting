#pragma once

#include "SDLppSurface.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <string>

class SDLppRenderer;

class SDLppTexture
{
public:
	SDLppTexture(SDL_Texture* texture);
	SDLppTexture(const SDLppTexture&) = delete;
	SDLppTexture(SDLppTexture&& texture);
	~SDLppTexture();

	SDL_Texture* GetHandle() const;
	SDL_Rect GetRect() const;

	SDLppTexture& operator=(const SDLppTexture&) = delete;
	SDLppTexture& operator=(SDLppTexture&& texture);

	static std::shared_ptr<SDLppTexture> FromFile(const SDLppRenderer& renderer, const std::string& filepath);
	static std::shared_ptr<SDLppTexture> FromSurface(const SDLppRenderer& renderer, const SDLppSurface& surface);

private:
	SDL_Texture* m_texture;
};