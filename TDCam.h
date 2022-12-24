#pragma once
#include "Misc.h"
class OrthoCamera : public MyCamera {
public:
void setProjection(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar
	)
	{
		this->projectionMatrix = glm::ortho(left,
			right,
			bottom,
			top,
			zNear,
			zFar);
	}
	void moveCam(glm::vec3* center) {
		viewMatrix = glm::translate(viewMatrix, *center);
	}
	void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

};
