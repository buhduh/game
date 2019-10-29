#include "artemis_memory.hpp"

int main() {
	GameMemory* gm = new GameMemory(gigabytes(2));
	auto one = gm->newStackArena(kilobytes(4));
	auto two = gm->newStackArena(kilobytes(6));
	auto three = gm->newStackArena(34);
	auto one1 = one->allocate(kilobytes(1));
	auto one2 = one->allocate(kilobytes(1));
	one->deallocate(three);	
	one->reset();
}
