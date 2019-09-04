#include <stdlib.h>
#include <cstring>

#include "memory/memory.hpp"

namespace memory {

//TODO align based on cache line
//TODO flags
Arena::Arena(size_t size) 
	: size(size)
	, blockCount(0)
{
	arena = malloc(size);
	blocks = (Block*) malloc(BLOCK_COUNT_MAX * sizeof(Block));
}

//TODO
Arena::~Arena() {
	free(arena);
	free(blocks);
}

void* Arena::allocate(size_t size) {
    uintptr_t toReturn = reinterpret_cast<uintptr_t>(arena);
	block_count_t i = 0;
    for(; i < blockCount; i++) {
        if(blocks[i].address - toReturn > size) break;
        toReturn = blocks[i].address + blocks[i].size;
    }
    if(i < blockCount) {
        memmove(blocks + i + 1, blocks + i, sizeof(Block)*(blockCount-i));
    }
    blocks[i] = Block{
        size: size,
        address: toReturn
    };
    blockCount++;
    return reinterpret_cast<void*>(toReturn);
}

void Arena::deallocate(void* ptr) {
    if(blockCount == 0) return;
	block_count_t i = 0;
	uintptr_t toMatch = reinterpret_cast<uintptr_t>(ptr);
    for(; i < blockCount; i++) {
        if(blocks[i].address == toMatch) {
            break;
        }
    }
    blockCount--;
    if(i < toMatch) {
        memmove(blocks + i, blocks + i + 1, sizeof(Block)*(blockCount-i));
    }
}

template <template <typename T> class Allocator, typename T>
Allocator<T> Arena::requestAllocator() {

}

}
