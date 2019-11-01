#ifndef ARTEMIS_CAMERA_HPP
#define ARTEMIS_CAMERA_HPP
/*
	Not really sure if this warrants its own header/class etc, but
	it will do for now
*/

#include "artemis_game.hpp"
#include "artemis_memory.hpp"

class Camera {
	public:
	static Camera* newCamera(IArena*, glm::vec3, glm::vec3, glm::vec3);
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	glm::mat4 view;
	void panLeft(float);
	void panRight(float);
	void panUp(float);
	void panDown(float);
	void panIn(float);
	void panOut(float);
	void rotateCCW(float);
	void rotateCW(float);
	private:
	void updateView();
	Camera(glm::vec3, glm::vec3, glm::vec3);
	Camera(const Camera&) = delete;
};

#endif
