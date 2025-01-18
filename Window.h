#pragma once
#include "SDL.h"

class Window
{
public:
	Window();
	~Window();

	int StartExecution();

private:
	SDL_Window* m_pWindow = nullptr;
	SDL_GLContext m_pGLContext = nullptr;

};

