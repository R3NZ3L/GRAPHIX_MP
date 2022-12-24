#pragma once
#include <glad/glad.h>	
#include <fstream>
#include <sstream>
#include <string>


class ShaderClass {
private:
	GLuint shaderProgram;

public:
	ShaderClass(std::string vertPath, std::string fragPath);

	void use();

	GLuint getShader();
	//gets uniform location
	GLint findUloc(const GLchar* src);
};
