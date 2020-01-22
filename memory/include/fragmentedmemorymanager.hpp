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
		Block* allocations;
		size_t maxAllocations;
		size_t allocationCount;
	};
};
#endif
