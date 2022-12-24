#pragma once
#include "Cameras.h"
class cam3p : public PerspectiveCamera {
public:
	void kbCallBack(GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods);
	inline void moveCam(glm::vec3* center) {
		cameraCenter = *center;
		cameraPos = cameraCenter - forward;
	}

};

