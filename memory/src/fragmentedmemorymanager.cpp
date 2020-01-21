#include "fragmentedmemorymanager.hpp"

namespace memory {

FragmentedMemoryManager::FragmentedMemoryManager(
	size_t size, 
	void* start, 
	Sector* const occupiedBlocks,
	size_t maxAllocations
)   
	: allocations(occupiedBlocks)
	, maxAllocations(maxAllocations)
	, allocationCount(0)
{
	Sector* sectorStart = reinterpret_cast<Sector*>(start);
	if(size < sizeof(Sector)) {
		size = 0;
	}
	sectorStart->blockDescriptor = Block{
		ptr: nullptr,
		size: size
	};
	head = sectorStart;
}

//Guarenteed to allocate at least size on the align boundary, may allocate more, 
//but the callee won't know the difference.
//returns nullptr if it could not allocate
//block size and ptr are always aligned to align
void* FragmentedMemoryManager::alignedAllocate(size_t size, size_t align) {
	if(allocationCount == maxAllocations) return nullptr;
	for(
		Sector* curr = head; 
		curr != nullptr; 
		curr = static_cast<Sector*>(curr->blockDescriptor.ptr)
	) 
	{
		if(curr->blockDescriptor.size < size) {
			continue;
		}
		//found a potential block, make sure it's adequate
		uintptr_t adjustedAddress = toUPtr(alignPointer(&curr, align));
		size_t diff = adjustedAddress - toUPtr(&curr);
		if(curr->blockDescriptor.size - diff >= size) {
			//TODO, modify the sectors here
			Block block = Block{
				ptr: toPtr(adjustedAddress),
				size: size
			};
			Sector sector;
			sector.blockDescriptor = block;
			allocations[allocationCount++] = sector;
			return nullptr;
		}
	}
	return nullptr;
}

//ptr becomes nullptr on success, does nothing if ptr is null
void FragmentedMemoryManager::deallocate(void* ptr) {

}

};
