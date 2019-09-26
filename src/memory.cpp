#include <assert.h>

#include "memory.hpp"
#include "platform.hpp"

GameMemory newGameMemory(size_t numBytes) {
	GameMemory toRet = GameMemory{};
	void *start = allocatePlatformMemory(numBytes);
	assert(start);
	toRet.size = numBytes;
	toRet.start = start;
}
