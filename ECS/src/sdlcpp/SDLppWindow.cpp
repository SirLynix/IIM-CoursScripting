#include "SDLppWindow.hpp"
#include <stdexcept>

SDLppWindow::SDLppWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags)
{
	m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
	if (!m_window)
		throw std::runtime_error(std::string("failed to create window: ") + SDL_GetError());
}

SDLppWindow::SDLppWindow(SDLppWindow&& window)
{
	m_window = window.m_window;
	window.m_window = nullptr;
}

SDLppWindow::~SDLppWindow()
{
	if (m_window)
		SDL_DestroyWindow(m_window);
}

SDLppRenderer SDLppWindow::CreateRenderer(Uint32 flags)
{
	return CreateRenderer(-1, flags);
}

SDLppRenderer SDLppWindow::CreateRenderer(int index, Uint32 flags)
{
	SDL_Renderer* renderer = SDL_CreateRenderer(m_window, index, flags);
	if (!renderer)
		throw std::runtime_error(std::string("failed to create renderer: ") + SDL_GetError());

	return SDLppRenderer(renderer);
}

SDLppWindow& SDLppWindow::operator=(SDLppWindow&& window)
{
	if (m_window)
		SDL_DestroyWindow(m_window);

	m_window = window.m_window;
	window.m_window = nullptr;

	return *this;
}
