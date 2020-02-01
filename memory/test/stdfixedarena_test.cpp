#include "catch2/catch.hpp"
#include "std_fixed_arena.hpp"
#include "game_memory.hpp"

void printDiagnostics(auto *vec) {	
	STD_LOG("size: " << vec->size());
	STD_LOG("max_size: " << vec->max_size());
	STD_LOG("capacity: " << vec->capacity());
}

TEST_CASE("Spike for now") {
	SECTION("basic usage");
		memory::GameMemory gm = memory::GameMemory(GIGABYTES(1), 1024);

		STD_LOG("gm.newSTDFixedArena<int>(128);");
		auto arena = gm.newSTDFixedArena<int>(128);
		STD_LOG("");
		
		STD_LOG("std::vector<int, memory::STDFixedArena<int>>(arena);");
		auto vec = std::vector<int, memory::STDFixedArena<int>>(arena);
		STD_LOG("");

/*
		STD_LOG("vec.push_back(4);");
		vec.push_back(4);
		printDiagnostics(&vec);
		STD_LOG("");

		STD_LOG("vec.push_back(4);");
		vec.push_back(4);
		printDiagnostics(&vec);
		STD_LOG("");

		STD_LOG("vec.push_back(4);");
		vec.push_back(4);
		printDiagnostics(&vec);
		STD_LOG("");

		STD_LOG("vec.push_back(4);");
		vec.push_back(4);
		printDiagnostics(&vec);
		STD_LOG("");

		STD_LOG("end");
*/
}
