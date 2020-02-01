#ifndef STD_FIXED_ARENA_HPP
#define STD_FIXED_ARENA_HPP

#include "memory_internal.hpp"
#include "fragmented_memory_manager.hpp"

/*
	Currently only intended for std::vectors, 
	should probably be restricted to basic types
	until I get more familiar with this gibberish
*/
namespace memory {

template<typename T>
class STDFixedArena {
	public:

	typedef T value_type;
	typedef size_t size_type;

	//size is num elems
	STDFixedArena(FragmentedMemoryManager* strategy, size_type size) 
	{
		maxSize = size;
		strategy = strategy;
		mem = (T*) strategy->allocate(size * sizeof(T));
		assert(mem);
	}

	~STDFixedArena() {
		STD_LOG("STDFixedArena destructor");
		if(strategy) STD_LOG("strategy exists");
		else STD_LOG("strategy does not exist");
		strategy->deallocate(mem);
	}

	T* allocate(size_type size) {
		if(size <= maxSize) {
			return mem;
		}
		strategy->deallocate(mem);
		mem = (T*) strategy->allocate(size * sizeof(T));
		assert(mem);
		maxSize = size;
		return mem;
	}

	//This particular arena only exists or it does not
	//all memory is created up front
	void deallocate(T* t,  size_type size) {}

	size_type max_size() {
		return  maxSize;
	}

	private:
	FragmentedMemoryManager* strategy;
	T* mem;
	size_type maxSize;
};

};
#endif
