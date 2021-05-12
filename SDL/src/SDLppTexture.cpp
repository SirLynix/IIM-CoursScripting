#include "SDLppTexture.hpp"
#include "SDLppRenderer.hpp"
#include <stdexcept>

SDLppTexture::SDLppTexture(SDL_Texture* texture) :
m_texture(texture)
{
}

SDLppTexture::SDLppTexture(SDLppTexture&& texture)
{
	m_texture = texture.m_texture;
	texture.m_texture = nullptr;
}

SDLppTexture::~SDLppTexture()
{
	if (m_texture)
		SDL_DestroyTexture(m_texture);
}

SDL_Texture* SDLppTexture::GetHandle() const
{
	return m_texture;
}

SDL_Rect SDLppTexture::GetRect() const
{
	int width, height;
	SDL_QueryTexture(m_texture, nullptr, nullptr, &width, &height);

	return SDL_Rect{ 0, 0, width, height };
}

SDLppTexture& SDLppTexture::operator=(SDLppTexture&& texture)
{
	if (m_texture)
		SDL_DestroyTexture(m_texture);

	m_texture = texture.m_texture;
	texture.m_texture = nullptr;

	return *this;
}

SDLppTexture SDLppTexture::FromFile(const SDLppRenderer& renderer, const std::string& filepath)
{
	return FromSurface(renderer, SDLppSurface::FromFile(filepath));
}

SDLppTexture SDLppTexture::FromSurface(const SDLppRenderer& renderer, const SDLppSurface& surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetHandle(), surface.GetHandle());
	if (!texture)
		throw std::runtime_error(std::string("failed to create texture from surface: ") + SDL_GetError());

	return SDLppTexture(texture);
}
