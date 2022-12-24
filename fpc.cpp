#include "Misc.h"
#include  "fpc.h"

void cam1p::kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Handler* hand = (Handler*)glfwGetWindowUserPointer(window);
	hand->player->kbCallBack(window, key, scancode, action, mods);
}
void cam1p::moveCam(glm::vec3* pos) {
	cameraPos += *pos;
	cameraCenter = cameraPos + forward;
	setView();
}

void cam1p::rotateCam(float deg) {
	viewMatrix = glm::rotate(viewMatrix,
		glm::radians(deg - 90),
		glm::vec3(0, 1, 0));
}
