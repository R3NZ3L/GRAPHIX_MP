#include "Misc.h"
#include "tpc.h"

void cam3p::kbCallBack(
	GLFWwindow* window, 
	int key, 
	int scancode, 
	int action, 
	int mods
) {
	Handler* hand = (Handler*)glfwGetWindowUserPointer(window);
	hand->player->kbCallBack(window,key,scancode,action,mods);
}
