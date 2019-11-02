#ifndef ARTEMIS_MEMORY_HPP
#define ARTEMIS_MEMORY_HPP

#include "artemis_game.hpp"

//I don't really have a clue what I'm doing with this memory stuff,
//place holding it for now with pure virtual classes

//Probably end up as the main game allocator
class IArena {
	public:
	virtual ~IArena() = default;
	virtual void* allocate(size_t) = 0; 
	virtual void deallocate(void*) = 0;
};

class StackArena : virtual public IArena {
	public:
	StackArena(size_t, void*);
	~StackArena() = default;
	void* allocate(size_t) override;
	void deallocate(void*) override;
	void reset();
	private:
	StackArena(StackArena&&) = delete;
	StackArena(const StackArena&) = delete;
	void* sp;
	const void* end;
	const void* start;
	const size_t size;
};

struct PoolHeader{
	PoolHeader* next;
};

class ConstantPoolArena : virtual public IArena {
	public:
	ConstantPoolArena(size_t, size_t, size_t, void*);
	void* allocate(size_t) override;
	void deallocate(void*) override;
	private:
	size_t numPools;
	size_t elemsPerPool;
	size_t poolSize;
	PoolHeader* head;
	void* start;
	void* end;
};

class GameMemory {
	public:
	GameMemory(size_t);
	~GameMemory();
	StackArena* newStackArena(size_t);
	ConstantPoolArena* newConstantPoolArena(
		size_t numPools, 
		size_t elemsPerPool, 
		size_t elemSize
	);
	private:
	GameMemory(GameMemory&&) = delete;
	GameMemory(const GameMemory&) = delete;
	void* marker;
	void* memory;
	void* end;

	//this will constantly increment every
	//arena request, not going to reclaim for now
	//TODO
	void* where;
};

class StupidArena : virtual public IArena {
	public:
	StupidArena() = default;
	~StupidArena() = default;
	void* allocate(size_t) override;
	void deallocate(void*) override;
	private:
	StupidArena(StupidArena&&) = delete;
	StupidArena(const StupidArena&) = delete;
};

#endif
