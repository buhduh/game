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
		void* tMem = strategy->allocate(size * sizeof(T));
		mem = reinterpret_cast<T*>(tMem);
	}

	~STDFixedArena() {
		strategy->deallocate(mem);
	}

	T* allocate(size_type size) {
		STD_LOG("std_fixed_arena allocating");
		STD_LOG("size: " << size);
		STD_LOG("maxSize: " << maxSize);
		//if(size <= maxSize) {
			//STD_LOG("size: " << size);
			//STD_LOG("maxSize: " << maxSize);
			//pretty sure i should never see this
			//assert(false);
		//}
		size_type oldSize = maxSize;
		while(size > maxSize) maxSize *= 2;
		void* tMem = strategy->allocate(maxSize * sizeof(T));
		assert(tMem);
		deallocate(mem, oldSize);
		mem = reinterpret_cast<T*>(tMem);
		return mem;
	}

	//TODO, don't think I need to do anything here...
	void deallocate(T* t,  size_type size) {
		strategy->deallocate(t);
	}

	size_type max_size() {
		return maxSize;
	}

	private:
	FragmentedMemoryManager* strategy;
	T* mem;
	size_type maxSize;
};

};
#endif
