#include "game_memory.hpp"

namespace memory {

GameMemory::GameMemory(size_t size, size_t maxArenas) {
	allocations = (Block*) malloc(sizeof(Block) * maxArenas);
	memory = malloc(size); 	
	strategy = new FragmentedMemoryManager(size, memory, allocations, maxArenas);
}

GameMemory::~GameMemory() {
	delete(strategy);
	free(allocations);
	free(memory);
}

};
