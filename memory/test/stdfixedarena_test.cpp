#include "catch2/catch.hpp"
#include "std_fixed_arena.hpp"
#include "game_memory.hpp"

TEST_CASE("Spike for now") {
	SECTION("Spiking");
		//auto arena = memory::STDFixedArena<int>();
		memory::GameMemory gm = memory::GameMemory(GIGABYTES(1), 1024);
		auto arena = gm.newSTDFixedArena<int>(1024);
		auto vec = std::vector<int, memory::STDFixedArena<int>>(arena);
}
