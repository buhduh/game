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
		: strategy(strategy)
		, maxSize(size)
	{
		mem = strategy->allocate(size * sizeof(T));
	}

	~STDFixedArena() {
		strategy->deallocate(mem);
	}

	T* allocate(size_type size) {
		return reinterpret_cast<T*>(mem);
	}

	//TODO, don't think I need to do anything here...
	void deallocate(T* t,  size_type size) {
	}

	size_type max_size() {
		return maxSize;
	}

	private:
	FragmentedMemoryManager* strategy;
	void* mem;
	size_type maxSize;
};

};
#endif
