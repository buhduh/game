#include <cstring>

#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "platform.hpp"

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

void* StupidArena::allocate(size_t size) {	
	return platform::allocatePlatformMemory(size);
}

void StupidArena::deallocate(void* ptr) {
	platform::deallocatePlatformMemory(ptr);
}

GameMemory::GameMemory(size_t size) {
	marker = platform::allocatePlatformMemory(size);
	uintptr_t tEnd = toUPtr(marker) + size;
	end = toPtr(tEnd);
	size_t align = platform::getCacheLineSize();
	memory = alignPointer(marker, align);
	where = memory;
}

GameMemory::~GameMemory() {
	platform::deallocatePlatformMemory(marker);
}

//this will absolutely need to change, just going to pass these out without
//any thought of reclaiming
//TODO
StackArena* GameMemory::newStackArena(size_t size) {
	StackArena sa(size, where);
	StackArena* toRet = (StackArena*) memcpy(where, &sa, sizeof(sa));
	uintptr_t tWhere = toUPtr(where);
	tWhere += size + sizeof(sa);
	where = alignPointer(toPtr(tWhere), platform::getCacheLineSize());
	return toRet;
}

//start is cache line aligned before calling this
StackArena::StackArena(size_t size, void* where) :
	size(size)
{
	uintptr_t tSP = toUPtr(where) + sizeof(*this);
	sp = alignPointer(toPtr(tSP), platform::getCacheLineSize());
	start = sp;
	end = toPtr(toUPtr(where) + size);
}

void* StackArena::allocate(size_t size) 
{
	assert(toUPtr(sp) + size <= toUPtr(end));
	void* toRet = sp;
	uintptr_t tSP = toUPtr(sp) + size;
	sp = alignPointer(toPtr(tSP), platform::getCacheLineSize());
	return toRet;
}

void StackArena::deallocate(void* ptr) {
	assert(toUPtr(start) <= toUPtr(ptr) && toUPtr(end) > toUPtr(ptr));
	sp = ptr;
}

void StackArena::reset() {
	sp = const_cast<void*>(start);
}
