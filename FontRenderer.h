#pragma once
#include <memory>
#include <string>
#include "glad/glad.h"
#include "Frame.h"

class Shader;

class FontRenderer
{
public:
	static FontRenderer& Get();

	void DrawText(const std::string& text, const RenderArea& renderArea);
private:
	FontRenderer();

	void InitializeOpenGL();

	GLuint m_FontAtlasId;
	GLuint m_QuadVerticesId;
	GLuint m_QuadIndicesId;
	GLuint m_QuadSizeUniformId;
	GLuint m_QuadVAO;
	GLuint m_TextSSBO;

	std::unique_ptr<Shader> m_pShader;
};

