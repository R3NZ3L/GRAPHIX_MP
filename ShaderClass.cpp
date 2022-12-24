
#include "ShaderClass.h"

//gets uniform location

ShaderClass::ShaderClass(std::string vertPath, std::string fragPath) {
	// Load .vert file
	std::fstream vertSrc(vertPath);
	std::stringstream vertBuff;
	vertBuff << vertSrc.rdbuf();
	std::string vertS = vertBuff.str();
	const char* v = vertS.c_str();

	// Load .frag file
	std::fstream fragSrc(fragPath);
	std::stringstream fragBuff;
	fragBuff << fragSrc.rdbuf();
	std::string fragS = fragBuff.str();
	const char* f = fragS.c_str();

	// Compile vertex shader
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &v, NULL);
	glCompileShader(vertShader);

	// Compile fragment shader
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &f, NULL);
	glCompileShader(fragShader);

	// Create shader program and link both vertex and fragment shaders
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertShader);
	glAttachShader(this->shaderProgram, fragShader);
	glLinkProgram(this->shaderProgram);
}

void ShaderClass::use() {
	glUseProgram(shaderProgram);
}

GLuint ShaderClass::getShader() {
	return this->shaderProgram;
}

GLint ShaderClass::findUloc(const GLchar* src) {
	GLint unif = glGetUniformLocation(this->shaderProgram, src);

	return unif;
}
