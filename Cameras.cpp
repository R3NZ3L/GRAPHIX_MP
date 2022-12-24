

#include "Cameras.h"
void MyCamera::setCameraPos(glm::vec3 c_Pos) {
	this->cameraPos = c_Pos;
}

void MyCamera::setCameraCenter(glm::vec3 c_Center) {
	this->cameraCenter = c_Center;
}

void MyCamera::setWorldUp(glm::vec3 w_Up) {
	this->worldUp = w_Up;
}
void MyCamera::setForward() {
	forward = glm::normalize(cameraCenter - cameraPos);
}

void MyCamera::setForward(glm::vec3* vecD) {
	forward = -(*vecD);
}

glm::vec3 MyCamera::getForward() {
	return this->forward;
}

void MyCamera::setView() {
	this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraCenter, this->worldUp);
}
glm::vec3 MyCamera::getCameraPos() {
	return this->cameraPos;
}

glm::vec3 MyCamera::getCameraCenter() {
	return this->cameraCenter;
}

glm::mat4 MyCamera::getProjectionMatrix() {
	return this->projectionMatrix;
}

glm::mat4 MyCamera::getViewMatrix() {
	return this->viewMatrix;
}

