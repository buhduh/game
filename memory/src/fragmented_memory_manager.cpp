#include <algorithm>
#include <cstring>
#include <stdint.h>

#include "fragmented_memory_manager.hpp"

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

//This thing is hideous...
void* FragmentedMemoryManager::allocate(size_t size) {
	if(allocationCount == maxAllocations) return nullptr;
	Block* prev = nullptr;
	Block* curr = head;
	for(;
		curr != nullptr;
		prev = curr,
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
			if(prev) {
				prev->next = curr->next;
			} else {
				head = head->next;
			}
		} else {
			void* newAddress = toPtr(toUPtr(curr) + size);
			Block* newBlock = reinterpret_cast<Block*>(newAddress);
			newBlock->next = curr->next;
			newBlock->size = newSize;
			if(curr == head) {
				head = newBlock;
			} else {
				prev->next = newBlock;
			}
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

void FragmentedMemoryManager::reconnectRegion(void* ptr) {
	Block* curr = head;
	while(curr) {
		if(curr == ptr) break;
		curr = curr->next;
	}
	if(!curr) return;
	if(curr->next == toPtr(toUPtr(curr) + curr->size)) {
		curr->size += curr->next->size;
		//can this ever logically be null?
		curr->next = curr->next->next;
	}
}

//does nothing if ptr is null
//should I zero it?
void FragmentedMemoryManager::deallocate(void* ptr) {
	if(!ptr) return;
	Block* found = nullptr;
	size_t i;
	for(i = 0; i < allocationCount; i++) {
		if(allocations[i].address != ptr) {
			continue;
		}
		found = allocations + i;
		break;
	}
	if(!found) return;
	Block* added = reinterpret_cast<Block*>(ptr);
	added->size = found->size;
	//can I merge this if and for together?
	//need to update head
	if(ptr < head || !head) {
		if(head) {
			Block* temp = head;
			head = added;
			added->next = temp;
		} else {
			head = added;
			added->next = nullptr;
		}
		reconnectRegion(ptr);
		removeAllocation(i);
		return;
	}
	for(Block* curr = head; curr != nullptr; curr = curr->next) {
		if(curr < ptr && curr->next > ptr) {
			Block* temp = curr->next;
			added->next = temp;
			curr->next = added;
			reconnectRegion(ptr);
			removeAllocation(i);
			return;
		}
	}
	//this should never be encountered
	assert(false);
}

void FragmentedMemoryManager::removeAllocation(size_t index) {
	assert(allocationCount > 0);
	allocations[index].size = SIZE_MAX;
	//not really sure if it's faster to memmove or sort
	//this thing..., if it's a problem would need
	//to benchmark I suppose.
	std::sort(
		allocations, allocations + allocationCount, 
		[](Block a, Block b) 
	{
		return a.address < b.address;
	});
	allocationCount -= 1;
}

};
