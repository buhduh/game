#include "catch2/catch.hpp"

#include "fragmentedmemorymanager.hpp"

class TestFragManager : public memory::FragmentedMemoryManager
{
	public:
	TestFragManager(
			size_t size, void* start, 
			size_t align = memory::CACHE_LINE_SZ
	) 
		: memory::FragmentedMemoryManager(size, start, align) 
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

#if 0
TEST_CASE("FragmentedMemoryManager allocations", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	void* alignedMem = memory::alignPointer(mem, memory::CACHE_LINE_SZ);

	SECTION("Allocations and deallocations when everything is aligned.");
	 	TestFragManager manager(KILOBYTES(1), alignedMem);
		void* foo = manager.allocate(KILOBYTES(1));

	free(mem);
};
#endif

TEST_CASE("FragmentedMemoryManager alignment initialization", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	void* alignedMem;

	SECTION("Passed pointer aligns to passed align.");
		alignedMem = memory::alignPointer(mem, memory::CACHE_LINE_SZ);
		assert(alignedMem);

	 	TestFragManager manager(KILOBYTES(1), alignedMem);
		REQUIRE(manager.getSector()->blockDescriptor.ptr == nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == KILOBYTES(1));

	 	manager = TestFragManager(MEGABYTES(1), alignedMem);
		REQUIRE(manager.getSector()->blockDescriptor.ptr == nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == MEGABYTES(1));

		manager = TestFragManager(MEGABYTES(43), alignedMem);
		REQUIRE(manager.getSector()->blockDescriptor.ptr == nullptr);
		REQUIRE(manager.getSector()->blockDescriptor.size == MEGABYTES(43));

		size_t align = sizeof(memory::Block);
		alignedMem = memory::alignPointer(mem, align);
		//the memory boundary aligns to align, and requests align count bytes
		manager = TestFragManager(align, alignedMem, align);
		REQUIRE(manager.getSector()->blockDescriptor.size == align);

		manager = TestFragManager(align/2, alignedMem, align);
		REQUIRE(manager.getSector()->blockDescriptor.size == 0);

		size_t size = sizeof(memory::Sector)/2;
		align = 2;
		alignedMem = memory::alignPointer(mem, align);
		assert(size > align && size < sizeof(memory::Sector));
		//the smallest allocation is sizeof(Sector);
		manager = TestFragManager(size, alignedMem, align);
		REQUIRE(manager.getSector()->blockDescriptor.size == 0);


	SECTION("Align is not a divisor of size and/or start.");
		
		alignedMem = memory::alignPointer(mem, 32);
		alignedMem = memory::toPtr(memory::toUPtr(alignedMem) + 2);
		manager = TestFragManager(KILOBYTES(1), alignedMem, 8);
		REQUIRE(manager.getSector()->blockDescriptor.size < KILOBYTES(1));

	free(mem);
};
