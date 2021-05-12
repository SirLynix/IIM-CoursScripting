#include "SDLppFont.hpp"
#include <stdexcept>

SDLppFont::SDLppFont(const std::string& fontPath, int size)
{
	m_font = TTF_OpenFont(fontPath.c_str(), size);
	if (!m_font)
		throw std::runtime_error("failed to open font " + fontPath + ": " + TTF_GetError());
}

SDLppFont::SDLppFont(SDLppFont&& font)
{
	m_font = font.m_font;
	font.m_font = nullptr;
}

SDLppFont::~SDLppFont()
{
	if (m_font)
		TTF_CloseFont(m_font);
}

SDLppSurface SDLppFont::RenderUTF8Blended(const std::string& text)
{
	SDL_Color white = { 255, 255, 255, 255 };
	return RenderUTF8Blended(text, white);
}

SDLppSurface SDLppFont::RenderUTF8Blended(const std::string& text, const SDL_Color& color)
{
	SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, text.c_str(), color);
	if (!surface)
		throw std::runtime_error(std::string("failed to render text: ") + TTF_GetError());

	return SDLppSurface(surface);
}

SDLppFont& SDLppFont::operator=(SDLppFont&& font)
{
	if (m_font)
		TTF_CloseFont(m_font);

	m_font = font.m_font;
	font.m_font = nullptr;

	return *this;
}
