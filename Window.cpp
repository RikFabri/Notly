#include "Window.h"
#include "glad/glad.h"

Window::Window()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("failed to init: %s", SDL_GetError());
		return;
	}

	// Set SDL to use OpenGL with Core Profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  // Depth buffer size

	m_pWindow = SDL_CreateWindow("Notly", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (m_pWindow == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return;
	}

	m_pGLContext = SDL_GL_CreateContext(m_pWindow);
	if (m_pGLContext == nullptr) {
		SDL_Log("Error creating OpenGL context: %s\n", SDL_GetError());
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to initialize GLAD\n");
		return;
	}

	SDL_Log("OpenGL Version: %s\n", glGetString(GL_VERSION));
}

Window::~Window()
{
	SDL_GL_DeleteContext(m_pGLContext);
	m_pGLContext = nullptr;
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	SDL_Quit();
}

int Window::StartExecution()
{
	bool quit = false;

	while (!quit)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}

	return 0;
}