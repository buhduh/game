#include "catch2/catch.hpp"
#include "game_memory.hpp"

TEST_CASE("GameMemory initialization") {
	SECTION("simple initialization");
		memory::GameMemory gameMemory = memory::GameMemory(GIGABYTES(1), 3);
}
