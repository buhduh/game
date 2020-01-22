#include "fragmentedmemorymanager.hpp"

namespace memory {

//for best results start should probably be aligned to a
//cache line
FragmentedMemoryManager::FragmentedMemoryManager(
	size_t size, 
	void* start, 
	Block* const occupiedBlocks,
	size_t maxAllocations
)   
	: allocations(occupiedBlocks)
	, maxAllocations(maxAllocations)
	, allocationCount(0)
{
	Block* block = reinterpret_cast<Block*>(start);
	if(size < sizeof(Block)) {
		size = 0;
	}
	block->ptr = nullptr;
	block->size = size;
	head = block;
}

//returns nullptr if it could not allocate
void* FragmentedMemoryManager::allocate(size_t size) {
	if(allocationCount == maxAllocations) return nullptr;
	Block* prev = head;
	Block* curr = head;
	for(;
		curr != nullptr; 
		prev = curr, curr = static_cast<Block*>(curr->ptr)
	) 
	{
		if(curr->size < size) {
			continue;
		}
		Block block = Block{
			ptr: curr,
			size: size
		};
		allocations[allocationCount++] = block;
		size_t newSize = curr->size - size;
		//pretty sure this will wrap if it goes 'negative'
		assert(newSize < curr->size);
		//curr was completely allocated
		if(newSize == 0) {
			prev->ptr = curr->ptr;
			return block.ptr;
		}
		//at end
		//TODO this doesn't work for initial allocations
		if(!curr->ptr) {
			prev->ptr = nullptr; //curr->ptr
		} else {
			prev->ptr = toPtr(toUPtr(curr->ptr) + size);
			curr->size = newSize;
		}
		return block.ptr;
	}
	return nullptr;
}

//ptr becomes nullptr on success, does nothing if ptr is null
void FragmentedMemoryManager::deallocate(void* ptr) {

}

};
