#pragma once
#include <memory>
#include <vector>
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
	unsigned int m_texture;
	unsigned int m_SSBO;

	std::unique_ptr<Shader> m_pShader = nullptr;

	std::string m_TextBuffer = "Hello world!";
	//std::string m_TextBuffer = "☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§";
	//std::vector<GLuint> m_TextBufferConverted = {0, 1, 16, 17, 32, 33, 64, 65};
	std::vector<GLuint> m_TextBufferConverted;
};

