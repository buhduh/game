#include "catch2/catch.hpp"

#include "fragmentedmemorymanager.hpp"

//don't really care if this leaks right now
bool makeManager() {
	void* mem = malloc(gigabytes(1));
	memory::FragmentedMemoryManager(gigabytes(1), mem);	
	return true;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( makeManager() == true );
}
