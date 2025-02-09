#include "FontRenderer.h"

#include <exception>
#include <vector>

#include "stbi_image.h"
#include "Shader.h"


FontRenderer& FontRenderer::Get()
{
	static FontRenderer renderer;
	return renderer;
}

void FontRenderer::DrawText(const std::string& text, const RenderArea& renderArea)
{
	static std::vector<GLuint> buffer;
	buffer.clear();

	for (const auto& ch : text)
	{
		buffer.emplace_back(ch);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TextSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, buffer.size() * sizeof(GLuint), &buffer[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_TextSSBO);

	glViewport(renderArea.x, renderArea.y, renderArea.width, renderArea.height);

	m_pShader->Use();
	m_pShader->SetUniform(renderArea.width, renderArea.height, m_QuadSizeUniformId);

	glBindVertexArray(m_QuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

FontRenderer::FontRenderer()
{
	InitializeOpenGL();
}

void FontRenderer::InitializeOpenGL()
{
	// TODO: rename the shader files
	m_pShader = std::make_unique<Shader>("vertexShader.vert", "fragmentShader.frag");
	m_QuadSizeUniformId = m_pShader->GetUniform("WindowSize");

	std::vector<GLuint> bop;
	const std::string str = "Hello world";
	for (auto it : str) { bop.emplace_back(it); }

	// Set up text buffer
	glGenBuffers(1, &m_TextSSBO);

	// unnecessary?
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_TextSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bop.size() * sizeof(GLuint), &bop[0], GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_TextSSBO);
	// ?


	// Set up font atlas
	glGenTextures(1, &m_FontAtlasId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FontAtlasId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int w, h, channels;
	auto* data = stbi_load("MonospaceAtlas1.png", &w, &h, &channels, 0);

	if (!data)
	{
		throw std::exception("Could not load font atlas!");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	// Set up mesh
	constexpr GLfloat vertices[] = {
		// positions		// uv coords
		 1.f,  1.f, 0.0f,	1.f, 1.f,	// top right
		 1.f, -1.f, 0.0f,	1.f, 0.f,	// bottom right
		-1.f, -1.f, 0.0f,	0.f, 0.f,	// bottom left
		-1.f,  1.f, 0.0f,	0.f, 1.f	// top left 
	};
	constexpr GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint vertexBuffer;
	GLuint indexBuffer;
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(m_QuadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
