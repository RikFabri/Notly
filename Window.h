﻿#pragma once
#include <memory>
#include <vector>
#include "SDL.h"
#include "Shader.h"
#include "Frame.h"

class Window
{
public:
	Window();
	~Window();

	int StartExecution();

private:
	void Draw();

	void CreateShader();
	void TempDrawTextWindow(const RenderArea& renderArea);
	void TestStuff();

	SDL_Window* m_pWindow = nullptr;
	SDL_GLContext m_pGLContext = nullptr;

	unsigned int m_ShaderProgram;
	unsigned int m_VAO;
	unsigned int m_texture;
	unsigned int m_SSBO;
	unsigned int m_WindowSizeUniform;

	std::unique_ptr<Shader> m_pShader = nullptr;

	std::string m_TextBuffer = "Hello world! Lorem ipsum dolor sit ames constectetur adipiscin elit. (from memory!)";
	//std::string m_TextBuffer = "☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§";
	//std::vector<GLuint> m_TextBufferConverted = {0, 1, 16, 17, 32, 33, 64, 65};
	std::vector<GLuint> m_TextBufferConverted;

	std::vector<IFrame*> m_Frames;
};

