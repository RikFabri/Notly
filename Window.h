#pragma once
#include <memory>
#include "SDL.h"
#include "Shader.h"

class Window
{
public:
	Window();
	~Window();

	int StartExecution();

private:
	void CreateShader();
	void TestStuff();

	SDL_Window* m_pWindow = nullptr;
	SDL_GLContext m_pGLContext = nullptr;

	unsigned int m_ShaderProgram;
	unsigned int m_VAO;

	std::unique_ptr<Shader> m_pShader = nullptr;
};

