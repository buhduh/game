#include "catch2/catch.hpp"

#include "fragmentedmemorymanager.hpp"

class TestFragManager : public memory::FragmentedMemoryManager
//class TestFragManager : memory::FragmentedMemoryManager
{
	public:
	TestFragManager(size_t size, void* start, memory::Sector* const allocations)
		: memory::FragmentedMemoryManager(size, start, allocations, 1024)
	{};
	memory::Sector* getSector() {return head;};
};

void* cleanAndReset(void* ptr, size_t size) {
	if(ptr) {
		free(ptr);
	}
	void* toRet = malloc(size);
	assert(toRet);
	return toRet;
}

void printBlock(memory::Block* block) {
	STD_LOG("block->ptr: " << block->ptr);
	STD_LOG("block->size: " << block->size);
};

TEST_CASE("FragmentedMemoryManager allocations", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	//not really too worried about this just yet
	void* tStore  = cleanAndReset(nullptr, MEGABYTES(1));
	//no freaking idea which casts I should use, may end up being
	//a good exercise...
	memory::Sector* const allocationStore = (memory::Sector* const)tStore;

	SECTION("Allocations and deallocations when everything is aligned.");
	 	TestFragManager manager(MEGABYTES(1), mem, allocationStore);
		void* foo = manager.alignedAllocate(KILOBYTES(1), memory::CACHE_LINE_SZ);

	free(mem);
};

TEST_CASE("FragmentedMemoryManager initialization", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	void* alignedMem;

	SECTION("Passed pointer aligns to passed align.");
		alignedMem = memory::alignPointer(mem, memory::CACHE_LINE_SZ);
		assert(alignedMem);

	 	TestFragManager manager(KILOBYTES(1), alignedMem, nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.ptr == nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == KILOBYTES(1));

	 	manager = TestFragManager(MEGABYTES(1), alignedMem, nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.ptr == nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == MEGABYTES(1));

		manager = TestFragManager(MEGABYTES(43), alignedMem, nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.ptr == nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == MEGABYTES(43));

		size_t align = sizeof(memory::Block);
		alignedMem = memory::alignPointer(mem, align);
		//the memory boundary aligns to align, and requests align count bytes
		manager = TestFragManager(align, alignedMem, nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == align);

		manager = TestFragManager(align/2, alignedMem, nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == 0);

		size_t size = sizeof(memory::Sector)/2;
		align = 2;
		alignedMem = memory::alignPointer(mem, align);
		assert(size > align && size < sizeof(memory::Sector));
		//the smallest allocation is sizeof(Sector);
		manager = TestFragManager(size, alignedMem, nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == 0);

	free(mem);
};
