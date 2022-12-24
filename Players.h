#pragma once

#include "Models.h"
#include "light.h"
#include <GLFW/glfw3.h>
/// <summary>
/// builder classes allow you to chain methods. similar to java builder classes
/// </summary>

class PlayerClass : public ModelClass
{
private:
	const float FORWARD_BACKWARD_MOVEMENT_SPEED = 0.3f;
	const float ASCEND_DESCEND_MOVEMENT_SPEED = 0.3f;
	const float LEFT_RIGHT_ROTATION_SPEED = 2.0f;
	const float LIGHT_SWAP_COOLDOWN = 0.2f;
	float timeOfLastLightStrengthSwap = 0.0f;
	enum class Intensity
	{
		LOW,
		MED,
		HI
	};
	Intensity str = Intensity::LOW;

public:
	glm::vec3 playerPos;
	glm::vec3 playerRot;
	float playerScale;
	lightBuilder* bulb;
	glm::mat4 transformationMatrix;
	glm::vec3 front;

	inline PlayerClass(std::string path,
		glm::vec3 pos,
		glm::vec3 rot,
		float scale) : ModelClass(path),
		playerPos(pos),
		playerRot(rot),
		playerScale(scale),
		bulb(new lightBuilder()),
		front(glm::vec3(0, 0, 0)),
		transformationMatrix(glm::mat4(1.0))
	{
		glm::vec3 src = pos;
		src.z -= 0.7;
		bulb
			->setLumens(1)
			->setAmbStr(1)
			->setSpecPhong(10)
			->setSpecStr(1)
			->setLightVec(&src)
			->setLightColor(new glm::vec3(1))
			->setAmbColor(new glm::vec3(1));
	}

	inline void placeUnifs(GLint* unifs)
	{
		switch (this->str)
		{
		case Intensity::LOW:
			bulb->setLumens(1);
			break;
		case Intensity::MED:
			bulb->setLumens(2);
			break;
		case Intensity::HI:
			bulb->setLumens(3);
			break;
		default:
			break;
		}

		bulb->placeUnifs(unifs);
	}

	inline void placeLight(GLint unif)
	{
		bulb->placeLight(unif);
	}

	void draw(GLuint shaderProgram)
	{
		glUseProgram(shaderProgram);
		glBindVertexArray(this->VAO);

		// Initialize transformation matrix, and assign position, scaling, and rotation
		transformationMatrix = glm::translate(glm::mat4(1),
			this->playerPos);

		// Scale
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(this->playerScale));

		// X-axis rotation
		transformationMatrix = glm::rotate(transformationMatrix,
			glm::radians(this->playerRot.x),
			glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
		// Y-axis rotation
		transformationMatrix = glm::rotate(transformationMatrix,
			glm::radians(this->playerRot.y),
			glm::normalize(glm::vec3(0, 0.5f, 0)));
		// Z-axis rotation
		transformationMatrix = glm::rotate(transformationMatrix,
			glm::radians(this->playerRot.z),
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

	float getDepth()
	{
		return this->playerPos.y;
	}

	void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		

		// Submarine Forward/Backward movement
		if (key == GLFW_KEY_W)
		{
			this->playerPos.x += FORWARD_BACKWARD_MOVEMENT_SPEED * front.x;
			this->playerPos.z -= FORWARD_BACKWARD_MOVEMENT_SPEED * front.z;
		}
		else if (key == GLFW_KEY_S)
		{
			this->playerPos.x -= FORWARD_BACKWARD_MOVEMENT_SPEED * front.x;
			this->playerPos.z += FORWARD_BACKWARD_MOVEMENT_SPEED * front.z;
		}

		// Submarine Ascend/Descend movement
		if (key == GLFW_KEY_Q && playerPos.y + 0.1f <= 0)
		{
			this->playerPos.y += ASCEND_DESCEND_MOVEMENT_SPEED;
		}
		else if (key == GLFW_KEY_E)
		{
			this->playerPos.y -= ASCEND_DESCEND_MOVEMENT_SPEED;
		}

		// Submarine Left/Right rotation movement
		if (key == GLFW_KEY_A)
		{
			this->playerRot.y += LEFT_RIGHT_ROTATION_SPEED;
		}
		else if (key == GLFW_KEY_D)
		{
			this->playerRot.y -= LEFT_RIGHT_ROTATION_SPEED;
		}

		if (key == GLFW_KEY_F &&
			(timeOfLastLightStrengthSwap == 0 ||
				glfwGetTime() - timeOfLastLightStrengthSwap > LIGHT_SWAP_COOLDOWN))
		{
			switch (this->str)
			{

			case Intensity::LOW:
				this->str = Intensity::MED;
				break;
			case Intensity::MED:
				this->str = Intensity::HI;
				break;
			case Intensity::HI:
				this->str = Intensity::LOW;
				break;
			default:
				this->str = Intensity::LOW;
				break;
			}

			timeOfLastLightStrengthSwap = glfwGetTime();
		}

		front.x = playerRot.y == 90 ? 0 : glm::cos(glm::radians(playerRot.y));
		front.z = playerRot.y == 90 ? 1 : glm::sin(glm::radians(playerRot.y));

		front = glm::normalize(front);

		glm::vec3 pos = playerPos;

		pos += glm::normalize(front); // we need to add an x offset because sub is not centered.

		bulb->setLightVec(&pos);

		glm::vec3 lightPos = playerPos;

		const float OFFSET = 0.8f;

		lightPos.z -= OFFSET;
		lightPos += front;

		bulb->setLightVec(&lightPos);
	}
};

