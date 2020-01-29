#ifndef ARTEMIS_GAME_HPP
#define ARTEMIS_GAME_HPP

#include <iostream>
#include <cassert>
#include <cstdint>
#include <stddef.h>
#include <stdint.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define KILOBYTES(SIZE) (1024 * (SIZE))
#define MEGABYTES(SIZE) (1024 * KILOBYTES(SIZE))
#define GIGABYTES(SIZE) (size_t(1024) * MEGABYTES(SIZE))

#define STD_LOG(MSG) (std::cout << MSG << std::endl)
#define STD_ERR(MSG) (std::cerr << MSG << std::endl)
#define QUIT(MSG) \
	STD_ERR(MSG); \
	exit(EXIT_FAILURE)

//I really hate defining this here but dealing with circular references
//When this fills up will begin ejecting older meshes that
//aren't in use
//these must match, change them concurrently
#define MAX_CONCURRENT_MESHES UINT16_MAX
typedef uint16_t meshcount_t;

inline void* toPtr(const uintptr_t ptr) {
	return reinterpret_cast<void*>(ptr);
}

inline uintptr_t toUPtr(const void* ptr) {
	return reinterpret_cast<uintptr_t>(ptr);
}

//this would go in a math library if i had one.
//probably when I need stuff not in glm.
static const float PI = glm::pi<float>();
static const float PI_OVER_2 = glm::pi<float>() / 2.0f;

#endif
