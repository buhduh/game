#ifndef ARTEMIS_GAME_HPP
#define ARTEMIS_GAME_HPP

#include <iostream>
#include <cassert>
#include <cstdint>
#include <stddef.h>
#include <stdint.h>

#define kilobytes(SIZE) (1024 * (SIZE))
#define megabytes(SIZE) (1024 * kilobytes(SIZE))
#define gigabytes(SIZE) (size_t(1024) * megabytes(SIZE))

#define STD_LOG(MSG) (std::cout << MSG << std::endl)

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

#endif
