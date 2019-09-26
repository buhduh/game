#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <stddef.h>

struct GameMemory {
	size_t size;
	void *start;
};

GameMemory newGameMemory(size_t);

#endif
