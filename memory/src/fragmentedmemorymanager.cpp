#include "fragmentedmemorymanager.hpp"

namespace memory {

FragmentedMemoryManager::FragmentedMemoryManager(
	size_t size, 
	void* start, 
	size_t align //= CACHE_LINE_SZ
)
{
	void* adjustedStart = alignPointer(start, align);
	uintptr_t sectorEndUPtr = alignAddress(toUPtr(adjustedStart) + size, align);
	//The given alignment pushes the sector end beyond the passed size,
	//reduce the sectorEnd by the alignment
	if(sectorEndUPtr - toUPtr(adjustedStart) > size) {
		sectorEndUPtr -= align;
	}
	size_t adjustedSize = sectorEndUPtr - toUPtr(adjustedStart);
	assert(adjustedSize % align == 0);
	Sector* sectorStart = static_cast<Sector*>(adjustedStart);
	sectorStart->blockDescriptor = Block{
		ptr: nullptr,
		size: adjustedSize
	};
	head = &sectorStart->blockDescriptor;
}

};
