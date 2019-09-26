#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>

struct Entity {
	glm::vec3 *vertices;
	glm::mat4 *modelTransform;
};

#endif
