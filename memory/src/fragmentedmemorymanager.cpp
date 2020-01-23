#include <algorithm>
#include <cstring>

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
	block->next = nullptr;
	block->size = size;
	head = block;
}

void* FragmentedMemoryManager::allocate(size_t size) {
	if(allocationCount == maxAllocations) return nullptr;
	Block** prev = &head;
	Block* curr = head;
	for(;
		curr != nullptr;
		prev = &curr,
		curr = curr->next
	)
	{
		if(curr->size < size) {
			continue;
		}
		Block allocBlock = Block{
			address: curr,
			size: size
		};
		allocations[allocationCount++] = allocBlock;
		size_t newSize = curr->size - size;
		//pretty sure this will wrap if it goes 'negative'
		assert(newSize < curr->size);
		//curr was completely allocated
		if(newSize == 0) {
			*prev = curr->next;
		} else {
			void* newAddress = toPtr(toUPtr(curr) + size);
			Block* newBlock = reinterpret_cast<Block*>(newAddress);
			newBlock->next = curr->next;
			newBlock->size = newSize;
			*prev = newBlock;
		}
		std::sort(
			allocations, allocations + allocationCount, 
			[](Block a, Block b) 
		{
			return a.address < b.address;
		});
		memset(allocBlock.address, 0, size);
		return allocBlock.address;
	}
	return nullptr;
}

//ptr becomes nullptr on success, does nothing if ptr is null
void FragmentedMemoryManager::deallocate(void* ptr) {
	if(!ptr) return;
	Block* found = nullptr;
	for(size_t i = 0; i < allocationCount; i++) {
		if(allocations[i].address != ptr) {
			continue;
		}
		found = allocations + i;
		break;
	}
}

};
