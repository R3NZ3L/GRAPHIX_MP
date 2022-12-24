#pragma once
#include "Cameras.h"

class cam1p : public PerspectiveCamera {
public:
	void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	void moveCam(glm::vec3* pos);
	void rotateCam(float deg);

};



