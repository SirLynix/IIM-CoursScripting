#pragma once

#include "SDLppSurface.hpp"
#include <SDL2/SDL_ttf.h>
#include <string>

class SDLppFont
{
public:
	SDLppFont(const std::string& fontPath, int size);
	SDLppFont(const SDLppFont&) = delete;
	SDLppFont(SDLppFont&& font);
	~SDLppFont();

	SDLppSurface RenderUTF8Blended(const std::string& text);
	SDLppSurface RenderUTF8Blended(const std::string& text, const SDL_Color& color);

	SDLppFont& operator=(const SDLppFont&) = delete;
	SDLppFont& operator=(SDLppFont&& font);

private:
	TTF_Font* m_font;
};