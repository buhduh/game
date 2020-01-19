#ifndef MEMORY_INTERNAL_HPP
#define MEMORY_INTERNAL_HPP

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>

#define KILOBYTES(SIZE) (1024 * (SIZE))
#define MEGABYTES(SIZE) (1024 * KILOBYTES(SIZE))
#define GIGABYTES(SIZE) (size_t(1024) * MEGABYTES(SIZE))

#define STD_LOG(MSG) (std::cout << MSG << std::endl)

namespace memory {

//to be truly portable this should probably
//be wrapped in some sort of platform layer
//good enough for now
static const size_t CACHE_LINE_SZ = 64;

//will likely have multiple meanings based on their implementation
//careful comments will be useful
struct Block {
	void* ptr;
	size_t size;
};

//unoccupied memory sectors 
//can save the metadata directly
//in their location
//the ocupied sector otherwise
union Sector {
	Block blockDescriptor;
	void* memory;
};

inline void* toPtr(const uintptr_t ptr) {
	return reinterpret_cast<void*>(ptr);
}

inline uintptr_t toUPtr(const void* ptr) {
	return reinterpret_cast<uintptr_t>(ptr);
}

inline uintptr_t alignAddress(uintptr_t addr, size_t align) {
	const size_t mask = align - 1;
	assert((align & mask) == 0);
	return (addr + mask) & ~mask;
}

inline void* alignPointer(void* ptr, size_t align) {
	const uintptr_t addr = toUPtr(ptr);
	const uintptr_t addrAligned = alignAddress(addr, align);
	return toPtr(addrAligned);
}

};

#endif
