#pragma once
#include <string>
#include "glad/glad.h"

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Use() const;

	void SetUniform(GLuint x, GLuint y, GLuint id) const;
	GLuint GetUniform(const std::string& name) const;

private:

	static GLuint CompileShader(const std::string& path, GLenum type);
	static GLuint LinkProgram(GLuint vertex, GLuint fragment);

	GLuint m_ID;
};

