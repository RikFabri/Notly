#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	GLuint vertex = CompileShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragment = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);

	m_ID = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Use() const
{
	glUseProgram(m_ID);
}

void Shader::SetUniform(GLuint x, GLuint y, GLuint id) const
{
	glUniform2i(id, x, y);
}

GLuint Shader::GetUniform(const std::string& name) const
{
	return glGetUniformLocation(m_ID, name.c_str());
}

GLuint Shader::CompileShader(const std::string& path, GLenum type)
{
	auto shader = glCreateShader(type);

	std::stringstream sourceStream{};
	{
		std::ifstream source(path);
		sourceStream << source.rdbuf();
	}

	auto sourceString = sourceStream.str();
	const auto rawSource = sourceString.c_str();

	glShaderSource(shader, 1, &rawSource, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		constexpr auto bufferSize = 512;
		char infoLog[bufferSize];

		glGetShaderInfoLog(shader, bufferSize, nullptr, infoLog);
		std::cerr << "CompileShader failed\n" << infoLog << std::endl;
	}

	return shader;
}

GLuint Shader::LinkProgram(GLuint vertex, GLuint fragment)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	// Check for linking errors
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		constexpr auto bufferSize = 512;
		char infoLog[bufferSize];
		glGetProgramInfoLog(program, bufferSize, nullptr, infoLog);
		std::cerr << "LinkProgram failed\n" << infoLog << std::endl;
	}

	return program;
}
