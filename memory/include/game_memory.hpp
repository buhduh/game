#ifndef GAME_MEMORY_HPP
#define GAME_MEMORY_HPP

#include "memory_internal.hpp"
#include "fragmented_memory_manager.hpp"
#include "std_fixed_arena.hpp"

namespace memory {
//TODO am I going to templatize the allocation strategies?
//eg FragmentedMemoryManager
	class GameMemory {
		public:
		GameMemory(size_t size, size_t maxArenas);
		~GameMemory();
		//size is num elems
		//TODO these should be pointers
		template<typename T>
		STDFixedArena<T> newSTDFixedArena(size_t size) {
			return STDFixedArena<T>(strategy, size);
		};
		private:
		FragmentedMemoryManager* strategy;
		Block* allocations;
		void* memory;
	};
};

#endif
