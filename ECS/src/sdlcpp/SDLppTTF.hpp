#pragma once

class SDLppTTF
{
public:
	SDLppTTF();
	SDLppTTF(const SDLppTTF&) = delete;
	SDLppTTF(SDLppTTF&&) = delete;
	~SDLppTTF();

	SDLppTTF& operator=(const SDLppTTF&) = delete;
	SDLppTTF& operator=(SDLppTTF&&) = delete;
};