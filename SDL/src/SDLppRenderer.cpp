#include "SDLppRenderer.hpp"
#include <stdexcept>

SDLppRenderer::SDLppRenderer(SDL_Renderer* renderer) :
m_renderer(renderer)
{
}

SDLppRenderer::SDLppRenderer(SDLppRenderer&& renderer)
{
	m_renderer = renderer.m_renderer;
	renderer.m_renderer = nullptr;
}

SDLppRenderer::~SDLppRenderer()
{
	if (m_renderer)
		SDL_DestroyRenderer(m_renderer);
}

void SDLppRenderer::Clear()
{
	SDL_RenderClear(m_renderer);
}

void SDLppRenderer::Copy(const SDLppTexture& texture)
{
	SDL_RenderCopy(m_renderer, texture.GetHandle(), nullptr, nullptr);
}

void SDLppRenderer::Copy(const SDLppTexture& texture, const SDL_Rect& dstRect)
{
	SDL_RenderCopy(m_renderer, texture.GetHandle(), nullptr, &dstRect);
}

void SDLppRenderer::Copy(const SDLppTexture& texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect)
{
	SDL_RenderCopy(m_renderer, texture.GetHandle(), &srcRect, &dstRect);
}

SDL_Renderer* SDLppRenderer::GetHandle() const
{
	return m_renderer;
}

void SDLppRenderer::Present()
{
	SDL_RenderPresent(m_renderer);
}

void SDLppRenderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

SDLppRenderer& SDLppRenderer::operator=(SDLppRenderer&& renderer)
{
	if (m_renderer)
		SDL_DestroyRenderer(m_renderer);

	m_renderer = renderer.m_renderer;
	renderer.m_renderer = nullptr;

	return *this;
}
