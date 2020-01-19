#include "fragmentedmemorymanager.hpp"

namespace memory {

FragmentedMemoryManager::FragmentedMemoryManager(
	size_t size, 
	void* start, 
	size_t align //= CACHE_LINE_SZ
)
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
	head = &sectorStart->blockDescriptor;
}

};
