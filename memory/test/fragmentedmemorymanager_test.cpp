#include <type_traits>

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
	STD_LOG("block->ptr: " << block->address);
	STD_LOG("block->size: " << block->size);
};

TEST_CASE("FragmentedMemoryManager allocations", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	//not really too worried about this just yet
	void* tStore  = cleanAndReset(nullptr, MEGABYTES(1));
	//no freaking idea which casts I should use, may end up being
	//a good exercise...

	SECTION("Initial allocations, everything should be stacking to the left.");
	{
		memory::Block* const allocationStore = (memory::Block* const)tStore;
		TestFragManager manager(KILOBYTES(5), mem, allocationStore);

		memory::Block* zeroethHead = manager.getBlock();
		void* firstAlloc = manager.allocate(KILOBYTES(1));
		REQUIRE(firstAlloc == (void*) zeroethHead);
			
		memory::Block* firstHead = manager.getBlock();
		void* secondAlloc = manager.allocate(KILOBYTES(3));
		REQUIRE((void*) firstHead == secondAlloc);
		REQUIRE(zeroethHead != firstHead);

		void* shouldBeNull = manager.allocate(KILOBYTES(4));
		REQUIRE(shouldBeNull == nullptr);

		memory::Block* secondHead = manager.getBlock();
		void* thirdAlloc = manager.allocate(KILOBYTES(1));
		REQUIRE((void*) secondHead == thirdAlloc);

		shouldBeNull = manager.allocate(1);
		REQUIRE(shouldBeNull == nullptr);
	}

	SECTION("Scrambling it to hell with deallocations and allocations");
	{
		mem = cleanAndReset(mem, MEGABYTES(32));
		//not really too worried about this just yet
		tStore = cleanAndReset(tStore, MEGABYTES(1));
		//TODO, this is fucking stupid, const blah blah
		//std::remove_const<memory::Block*>::type allocationStore;
		//allocationStore = (memory::Block* const)static_cast<memory::Block*>(tStore);
		//TODO do I care, like at all, to get this working? 
		//probably "good" c++ practice...
		memory::Block* const allocationStore = (memory::Block* const)tStore;

		/*
		following nomenclature for intended structure:
		[] -> empty
		[x|0] -> 1KB allocated on left, 1KB free on right
		[xx|x|o|xxx] -> 4 allocations, 2KB on left, 3KB on right, etc
		*/
		TestFragManager	manager(KILOBYTES(10), mem, allocationStore);
		//[x|ooooooooo]
		void* firstAlloc = manager.allocate(KILOBYTES(1));
		REQUIRE(manager.getBlock() == 
			memory::toPtr(memory::toUPtr(firstAlloc) + KILOBYTES(1))
		);
		//[x|x|oooooooo]
		void* secondAlloc = manager.allocate(KILOBYTES(1));
		//[x|x|x|ooooooo]
		void* thirdAlloc = manager.allocate(KILOBYTES(1));
		REQUIRE(manager.getBlock() == 
			memory::toPtr(memory::toUPtr(thirdAlloc) + KILOBYTES(1))
		);
		//[x|o|x|ooooooo]
		manager.deallocate(secondAlloc);
		REQUIRE(manager.getBlock() == secondAlloc);
		REQUIRE(manager.getBlock()->next == memory::toPtr(memory::toUPtr(secondAlloc) + KILOBYTES(2)));
		//[x|o|x|xxx|oooo]
		void* fourthAlloc = manager.allocate(KILOBYTES(3));
		REQUIRE(memory::toUPtr(thirdAlloc) + KILOBYTES(1) == memory::toUPtr(fourthAlloc));
		REQUIRE(manager.getBlock() == secondAlloc);
		REQUIRE(
			manager.getBlock()->next == 
			memory::toPtr(memory::toUPtr(manager.getBlock()) + KILOBYTES(5))
		);
		//[x|o|x|ooooooo]
		manager.deallocate(fourthAlloc);
		REQUIRE(manager.getBlock()->next == fourthAlloc);
		
		void* fifthAlloc = manager.allocate(KILOBYTES(7));
		REQUIRE(fourthAlloc == fifthAlloc);
		REQUIRE(manager.getBlock() == secondAlloc);
		REQUIRE(manager.getBlock()->next == nullptr);
	}

	free(mem);
	free(tStore);
};

TEST_CASE("FragmentedMemoryManager initialization", "[fragmentedmemorymanager]" ) {

	void* mem = cleanAndReset(nullptr, MEGABYTES(32));
	void* alignedMem;

	SECTION("Passed pointer aligns to passed align.");
		alignedMem = memory::alignPointer(mem, memory::CACHE_LINE_SZ);
		assert(alignedMem);

	 	TestFragManager manager(KILOBYTES(1), alignedMem, nullptr);
		REQUIRE(manager.getBlock()->address == nullptr);
		REQUIRE(manager.getBlock()->size == KILOBYTES(1));

	 	manager = TestFragManager(MEGABYTES(1), alignedMem, nullptr);
		REQUIRE(manager.getBlock()->address == nullptr);
		REQUIRE(manager.getBlock()->size == MEGABYTES(1));

		manager = TestFragManager(MEGABYTES(43), alignedMem, nullptr);
		REQUIRE(manager.getBlock()->address == nullptr);
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
