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

//up probably doesn't need to be set every time, only if it becomes
//a problem
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

//TODO, I'm sure there's a better way to do this....
void Camera::rotateCCW(float radians) {
	auto alignVec = center - eye;
	auto rotVec = glm::rotate(alignVec, radians, up);
	center += alignVec - rotVec;
	updateView();
}

//TODO, I'm sure there's a better way to do this....
void Camera::rotateCW(float radians) {
	auto alignVec = center - eye;
	auto rotVec = glm::rotate(alignVec, radians, -1.0f * up);
	center += alignVec - rotVec;
	updateView();
}

//TODO, I'm sure there's a better way to do this....
void Camera::rotateIn(float radians) {
	auto rotVec = glm::rotate(up, radians, (center - eye));
	up += rotVec;
	updateView();
}

//TODO, I'm sure there's a better way to do this....
void Camera::rotateOut(float radians) {
	auto rotVec = glm::rotate(up, radians, (eye - center));
	up += rotVec;
	updateView();
}

void Camera::snapToOrigin() {
	center = glm::vec3(0.0f);
	updateView();
}
