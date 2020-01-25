#ifndef STD_FIXED_ARENA_HPP
#define STD_FIXED_ARENA_HPP

#include "memory_internal.hpp"

/*
	Currently only intended for std::vectors, 
	should probably be restricted to basic types
	until I get more familiar with this gibberish
*/
namespace memory {

template<typename T>
class STDFixedArena {
	public:

	STDFixedArena() {
		foo = 3;
	}

	typedef T value_type;
	typedef size_t size_type;

	T* allocate(size_type size) {
		return (T*) malloc(1024 * sizeof(T));	
	}

	void deallocate(T* t,  size_type size) {
		free(t);
	}

	size_type max_size() {
		return 1024;
	}

	private:
	//TODO just a test var
	int foo;
};

};
#endif
