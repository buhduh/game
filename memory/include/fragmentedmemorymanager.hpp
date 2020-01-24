#ifndef FRAGMENTED_MEMORY_MANAGER_HPP
#define FRAGMENTED_MEMORY_MANAGER_HPP

#include "memory_internal.hpp"

namespace memory {
	class FragmentedMemoryManager {
		public:
		/*
			Memory allocations happen directly on passed start
		*/
		FragmentedMemoryManager(
			size_t size, 
			void* start, 
			Block* const allocations, 
			size_t const maxAllocations
		);
		void* allocate(size_t);
		void deallocate(void*);
		//only intended to be derived by test classes
		//but you do you
		protected:
		//a linked list
		Block* head;
		private:
		//Not quite sure why I didn't make
		//the allocations a linked list.
		//If sorting this every allocation becomes a
		//bottleneck, perhaps I should
		Block* allocations;
		size_t maxAllocations;
		size_t allocationCount;
		void removeAllocation(size_t);
	};
};
#endif
