#ifndef WORLD_HPP
#define WORLD_HPP

#include "entity.hpp"

//is this a god object?
struct World {
	Entity *entities;
	glm::vec3 *vertices;
	glm::mat4 *modelTransforms;
	alignas(16) glm::mat4 viewTransform;
	alignas(16) glm::mat4 projectionTransform;
};

#endif
