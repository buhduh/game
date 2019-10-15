#include <assert.h>

#include "artemis_memory.hpp"
#include "platform.hpp"

void* StupidArena::allocate(size_t size) {	
	return platform::allocatePlatformMemory(size);
}

void StupidArena::deallocate(void* ptr) {
	platform::deallocatePlatformMemory(ptr);
}

//I don't think size matters right now since I'm just allocating with
//the built in OS allocators
IArena* newArena(size_t size) {
	return new StupidArena();
}

void destroyArena(IArena* arena) {
	delete(arena);
}
