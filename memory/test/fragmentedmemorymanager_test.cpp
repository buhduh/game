#include "catch2/catch.hpp"

#include "fragmentedmemorymanager.hpp"

class TestFragManager : public memory::FragmentedMemoryManager
//class TestFragManager : memory::FragmentedMemoryManager
{
	public:
	TestFragManager(size_t size, void* start, memory::Block* const allocations)
		: memory::FragmentedMemoryManager(size, start, allocations, 1024)
	{};
	memory::Block* getBlock() {return head;};
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
	memory::Block* const allocationStore = (memory::Block* const)tStore;

	SECTION("Initial allocations, everything should be stacking to the left.");

	 	TestFragManager manager(KILOBYTES(5), mem, allocationStore);

		memory::Block* oldHead = manager.getBlock();
		void* firstAlloc = manager.allocate(KILOBYTES(1));
		REQUIRE(firstAlloc == (void*) oldHead);

		memory::Block* firstHead = manager.getBlock();
		void* secondAlloc = manager.allocate(KILOBYTES(1));
		REQUIRE((void*) firstHead == secondAlloc);

		void* shouldBeNull = manager.allocate(KILOBYTES(4));
		REQUIRE(shouldBeNull == nullptr);

	free(mem);
};

TEST_CASE("FragmentedMemoryManager initialization", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	void* alignedMem;

	SECTION("Passed pointer aligns to passed align.");
		alignedMem = memory::alignPointer(mem, memory::CACHE_LINE_SZ);
		assert(alignedMem);

	 	TestFragManager manager(KILOBYTES(1), alignedMem, nullptr);
		REQUIRE(manager.getBlock()->ptr == nullptr);
		REQUIRE(manager.getBlock()->size == KILOBYTES(1));

	 	manager = TestFragManager(MEGABYTES(1), alignedMem, nullptr);
		REQUIRE(manager.getBlock()->ptr == nullptr);
		REQUIRE(manager.getBlock()->size == MEGABYTES(1));

		manager = TestFragManager(MEGABYTES(43), alignedMem, nullptr);
		REQUIRE(manager.getBlock()->ptr == nullptr);
		REQUIRE(manager.getBlock()->size == MEGABYTES(43));

		size_t align = sizeof(memory::Block);
		alignedMem = memory::alignPointer(mem, align);
		//the memory boundary aligns to align, and requests align count bytes
		manager = TestFragManager(align, alignedMem, nullptr);
		REQUIRE(manager.getBlock()->size == align);

		manager = TestFragManager(align/2, alignedMem, nullptr);
		REQUIRE(manager.getBlock()->size == 0);

		size_t size = sizeof(memory::Block)/2;
		align = 2;
		alignedMem = memory::alignPointer(mem, align);
		assert(size > align && size < sizeof(memory::Block));
		//the smallest allocation is sizeof(Block);
		manager = TestFragManager(size, alignedMem, nullptr);
		REQUIRE(manager.getBlock()->size == 0);

	free(mem);
};
