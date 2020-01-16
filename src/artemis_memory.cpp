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

//TODO
void GameMemory::deallocateArena(IArena* arena) {
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
	StackArena sa(size, where, this);
	StackArena* toRet = (StackArena*) memcpy(where, &sa, sizeof(sa));
	uintptr_t tWhere = toUPtr(where);
	tWhere += size + sizeof(sa);
	where = alignPointer(toPtr(tWhere), platform::getCacheLineSize());
	return toRet;
}

StackArena::~StackArena() {
	STD_LOG("~StackArena not implemented, TODO");
}

//start is cache line aligned before calling this
//this is a little rough around the edges, doesn't quite work for
//allocations EXACTLY equal to initialized size due to alignment
StackArena::StackArena(size_t size, void* where, GameMemory* gMemory) 
	: size(size)
	, gameMemory(gMemory)
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

ConstantPoolArena* GameMemory::newConstantPoolArena(
	size_t numPools, 
	size_t elemsPerPool,
	size_t elemSize
)
{
	size_t poolSize = alignAddress(
		(elemSize * elemsPerPool), platform::getCacheLineSize()
	);
	PoolHeader ph;
	void* start = where;
	for(size_t i = 0; i < numPools - 1; ++i) {
		ph.next = (PoolHeader*) toPtr(toUPtr(where) + poolSize);
		memcpy(where, &ph, sizeof(ph));
		where = (void*) ph.next;
	}
	{
		//the last poolheader needs to point to null
		//and where needs to be incremented
		ph.next = nullptr;
		memcpy(where, &ph, sizeof(ph));
		where = toPtr(toUPtr(where) + poolSize);
	}
	ConstantPoolArena temp = ConstantPoolArena(
		numPools, elemsPerPool, 
		poolSize, start
	);
	ConstantPoolArena* toRet = (ConstantPoolArena*) memcpy(
		where, &temp, sizeof(ConstantPoolArena)
	);
	where = toPtr(toUPtr(where) + sizeof(ConstantPoolArena));
	where = alignPointer(where, platform::getCacheLineSize());
	return toRet;
}

void StackArena::reset() {
	sp = const_cast<void*>(start);
}

ConstantPoolArena::ConstantPoolArena(
	size_t numPools, 
	size_t elemsPerPool,
	size_t poolSize,
	void* _head
)
	: numPools(numPools)
	, elemsPerPool(elemsPerPool)
	, poolSize(poolSize)
{
	head = (PoolHeader*) _head;
	start = _head;
	end = toPtr(toUPtr(start) + numPools * poolSize);
}

void* ConstantPoolArena::allocate(size_t size) {
	assert(size <= poolSize);
	assert(head);
	void* toRet = (void*) head;
	head = head->next;
	return toRet;
}

void ConstantPoolArena::deallocate(void* _ptr) {
	assert(toUPtr(start) <= toUPtr(_ptr) && toUPtr(end) > toUPtr(_ptr));
	PoolHeader* ptr = (PoolHeader*) _ptr;
	PoolHeader* temp = head;
	head = ptr;
	ptr->next = temp;
}
