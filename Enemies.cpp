#include "Enemies.h"

EnemyClass::EnemyClass(std::string path, glm::vec3 pos, glm::vec3 rot, float scale) : ModelClass(path),
enemyPos(pos),
enemyRot(rot),
enemyScale(scale) {}

void EnemyClass::draw(GLuint shaderProgram)
{
	glUseProgram(shaderProgram);
	glBindVertexArray(this->VAO);

	// Initialize transformation matrix, and assign position, scaling, and rotation
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), this->enemyPos);
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(this->enemyScale));

	// X-axis rotation
	transformationMatrix = glm::rotate(transformationMatrix,
		glm::radians(this->enemyRot.x),
		glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

	// Y-axis rotation
	transformationMatrix = glm::rotate(transformationMatrix,
		glm::radians(this->enemyRot.y),
		glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

	// Z-axis rotation
	transformationMatrix = glm::rotate(transformationMatrix,
		glm::radians(this->enemyRot.z),
		glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

	// Initialize transformation location, and assign transformation
	unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

	glActiveTexture(GL_TEXTURE0);
	GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
	glBindTexture(GL_TEXTURE_2D, this->textures[0]);
	glUniform1i(tex0Address, 0);

	if (withNormals)
	{
		glActiveTexture(GL_TEXTURE1);
		GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
		glBindTexture(GL_TEXTURE_2D, this->textures[1]);
		glUniform1i(tex1Address, 1);
	}

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size() / 5);
}
