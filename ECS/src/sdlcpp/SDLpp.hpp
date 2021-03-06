#pragma once

#include <SDL2/SDL.h>

class SDLpp
{
public:
	SDLpp();
	SDLpp(const SDLpp&) = delete;
	SDLpp(SDLpp&&) = delete;
	~SDLpp();

	const Uint8* GetKeyboardState(int* numkeys = nullptr) const;

	Uint64 GetPerformanceCounter() const;
	Uint64 GetPerformanceFrequency() const;

	bool PollEvent(SDL_Event& event);

	SDLpp& operator=(const SDLpp&) = delete;
	SDLpp& operator=(SDLpp&&) = delete;
};