#include "Window.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image.h"
#include "Shader.h"

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

	m_pWindow = SDL_CreateWindow("Notly", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 512, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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

	CreateShader();
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

		TestStuff();
	}

	return 0;
}

#include <iostream>

void Window::CreateShader()
{
	m_pShader = std::make_unique<Shader>("vertexShader.vert", "fragmentShader.frag");

	// Create SSBO text buffer thing
	for (const auto& ch : m_TextBuffer)
	{
		m_TextBufferConverted.emplace_back(ch);
		std::cout << GLuint(ch) << std::endl;
	}
	glGenBuffers(1, &m_SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_TextBufferConverted.size() * sizeof(GLuint), &m_TextBufferConverted[0], GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SSBO);

	// create texture
	glGenTextures(1, &m_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	int w, h, channels;
	unsigned char* data = stbi_load("MonospaceAtlas.png", &w, &h, &channels, 0);
	if (!data)
	{
		SDL_Log("failed to load atlas");
		return;
	}
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);



	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions		// uv coords
		 1.f,  1.f, 0.0f,	1.f, 1.f, // top right
		 1.f, -1.f, 0.0f,	1.f, 0.f, // bottom right
		-1.f, -1.f, 0.0f,	0.f, 0.f, // bottom left
		-1.f,  1.f, 0.0f,	0.f, 1.f // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Window::TestStuff()
{
	int w, h;
	SDL_GetWindowSize(m_pWindow, &w, &h);


	//glViewport(0, 0, w, h / 2);
	glClearColor(0.2f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_pShader->Use();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SSBO);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glClearColor(0.f, 0.f, 0.f, 1.f);
	//glViewport(0, h / 2, w, h / 2);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(m_pWindow);
}
