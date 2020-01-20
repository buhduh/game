#include "fragmentedmemorymanager.hpp"

namespace memory {

FragmentedMemoryManager::FragmentedMemoryManager(
	size_t size, 
	void* start, 
	size_t align //= CACHE_LINE_SZ
) : alignment(align)
{
	//align is power of 2
	const size_t mask = align - 1;
	assert((align & mask) == 0);
	void* adjustedStart = alignPointer(start, align);
	//uintptr_t sectorEndUPtr = alignAddress(toUPtr(adjustedStart) + size, align);
	//The given alignment pushes the sector end beyond the passed size
	if(adjustedStart != start) {
		size -= (toUPtr(adjustedStart) - toUPtr(start));
	}
	Sector* sectorStart = reinterpret_cast<Sector*>(adjustedStart);
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
//should this 0 the memory?
/*
void* FragmentedMemoryManager::allocate(size_t size) {
	size_t adjustedSize = alignAddress(size, alignment);
	for(Block* curr = head; curr != nullptr; curr = static_cast<Block*>(curr->ptr)) {
		if(curr->size < adjustedSize) {
			continue;
		}
		//We found a block
#if 0
		//pretty sure this scenario is a bit more tricky, doing it later
		//this also needs to handle the scenario when the space between blocks isn't
		//large enough
		if(curr->size == adjustedSize) {
			return found block			
		}
#endif
		void* toRet = curr->ptr;
	}
	return nullptr;
}
*/

//ptr becomes nullptr on success, does nothing if ptr is null
void FragmentedMemoryManager::deallocate(void* ptr) {

}

};
