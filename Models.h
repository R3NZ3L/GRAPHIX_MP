#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>




class ModelClass
{
protected:
	std::string objPath;
	std::vector<GLfloat> vertexData;
	std::vector<GLuint> textures;
	bool withNormals = false;
	GLuint VAO, VBO;

public:
	inline ModelClass(std::string path) : objPath(path),
		VAO(NULL),
		VBO(NULL) {}

	void loadObj();

	void attachTexture(std::string texPath, GLint format);
	void attachNormalTexture(std::string texPath, GLint format);
	void createVAO_VBO();

	inline GLuint getVAO()
	{
		return this->VAO;
	}

	inline GLuint getBaseTexture()
	{
		return this->textures[0];
	}

	inline GLuint getNormals()
	{
		return this->textures[1];
	}

	inline std::vector<GLfloat> getVertexData()
	{
		return this->vertexData;
	}
};

