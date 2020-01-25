#include "catch2/catch.hpp"
#include "std_fixed_arena.hpp"

TEST_CASE("Spike for now") {
	SECTION("Spiking");
		auto arena = memory::STDFixedArena<int>();
		auto vec = std::vector<int, memory::STDFixedArena<int>>(arena);
}
