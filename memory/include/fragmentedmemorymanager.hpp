#ifndef FRAGMENTED_MEMORY_MANAGER_HPP
#define FRAGMENTED_MEMORY_MANAGER_HPP

#include "memory_internal.hpp"

namespace memory {
	class FragmentedMemoryManager {
		public:
		/*
			Memory allocations happen directly on passed start
			and are possible up to size dependant on align when allocating
		*/
		FragmentedMemoryManager(
			size_t size, 
			void* start, 
			Sector* const allocations, 
			size_t const maxAllocations
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
		void* alignedAllocate(size_t sz, size_t align);
		void deallocate(void*);
		//only intended to be derived by test classes
		//but you do you
		protected:
		//a linked list
		Sector* head;
		private:
		//sets Sector to the Sector BEFORE the found location
		//for easier list updates
		//DOES NOT allocate, allocate must still be
		//called
		//I don't think I need this
		//Result canAllocate(Sector* loc, size_t size, size_t align);
		Sector* allocations;
		size_t maxAllocations;
		size_t allocationCount;
	};
};
#endif
