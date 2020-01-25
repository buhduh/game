#ifndef GAME_MEMORY_HPP
#define GAME_MEMORY_HPP

#include "fragmented_memory_manager.hpp"
#include "memory_internal.hpp"

namespace memory {
//TODO am I going to templatize the allocation strategies?
//eg FragmentedMemoryManager
	class GameMemory {
		public:
		//GameMemory(size_t size, size_t maxArenas = MAX_MEMORY_ARENAS);
		GameMemory(size_t size, size_t maxArenas);
		~GameMemory();
		private:
		FragmentedMemoryManager* strategy;
		Block* allocations;
		void* memory;
	};
};

#endif
