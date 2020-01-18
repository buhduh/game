#ifndef FRAGMENTED_MEMORY_MANAGER_HPP
#define FRAGMENTED_MEMORY_MANAGER_HPP

#include "memory_internal.hpp"

namespace memory {
	class FragmentedMemoryManager {
		public:
		/*
			Gaurantees that allocated memory will NEVER be larger than size
			for best results align should be a divisor of size and start 
			otherwise start will be placed on the next greatest alignment address
			and size will be shifted smaller to fit the given alignment
		*/
		FragmentedMemoryManager(
			size_t size, void* start, size_t align = CACHE_LINE_SZ
		);
	/*
		possible scenarios
		let x be occupied and o be free
		left is 0x0 right is larger
		1.  [xxxx|ooooooooo], free sector grows, but count stays same
		2.  [ooooooo|xxxxxx], same as 1
		3.  [ooooo|xxxxx|ooooo], 2 free sectors merge into a single sector
		4.  [xxxxxx|xxxxxx|xxxxxxx], middle sector freed, new free sector
		5.  [xxxxxx|xxxxxx|xxxxxxx], left sector freed, new free sector, 
			basically an occupied sector surrounded by occupied sectors
			edge case where to be freed is at start
		6.  [xxxxxx|xxxxxx|xxxxxxx], right sector freed, new free sector,
			edge case, freed sector is at end of buffer
	*/
		//Since these are ordered, an optimization can be found when looking for
		//sectors O(n) -> O(log(n))
		void* allocate(size_t);
		void deallocate(void*);
		//only intended to be derived by test classes
		//but you do you
		protected:
		//a linked list
		Block* head;	
	};
};
#endif
