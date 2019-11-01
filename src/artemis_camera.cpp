#include "artemis_camera.hpp"

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	: eye(eye)
	, center(center)
	, up(up)
{
	updateView();
}

Camera* Camera::newCamera(
	IArena* arena, glm::vec3 eye, 
	glm::vec3 center, glm::vec3 up) 
{	
	Camera* toRet = (Camera*) arena->allocate(sizeof(Camera));
	*toRet = Camera(eye, center, up);
	return toRet;
}

void Camera::updateView() {
	view = glm::lookAt(eye, center, up);
	up = glm::vec3(view[0][1], view[1][1], view[2][1]);
}

//distance will need to be modified once a sane coordinate system is
//introduced
//updateView should probably be pulled out
void Camera::panLeft(float dis) {
	glm::vec3 left = glm::normalize(glm::cross(up, center - eye));
	eye += left * dis;
	center += left * dis;
	updateView();
}

void Camera::panRight(float dis) {
	glm::vec3 left = glm::normalize(glm::cross(up, center - eye));
	auto vec = left * dis;
	eye -= vec;
	center -= vec;
	updateView();
}

void Camera::panUp(float dis) {
	auto vec = up * dis;
	eye += vec;
	center += vec;
	updateView();
}

void Camera::panDown(float dis) {
	auto vec = up * dis;
	eye -= vec;
	center -= vec;
	updateView();
}

void Camera::panIn(float dis) {
	auto vec = center - eye;
	eye -= vec * dis;
	center -= vec * dis;
	updateView();
}

void Camera::panOut(float dis) {
	auto vec = center - eye;
	eye += vec * dis;
	center += vec * dis;
	updateView();
}

void Camera::rotateCCW(float radians) {
}

void Camera::rotateCW(float radians) {
}

