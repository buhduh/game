#include "artemis_memory.hpp"

int main() {
	GameMemory* gm = new GameMemory(gigabytes(1));
	auto pa = gm->newConstantPoolArena(10, 100, sizeof(void*));
	auto data1 = pa->allocate(sizeof(void*) * 100);
	auto data2 = pa->allocate(sizeof(void*) * 100);
	auto data3 = pa->allocate(sizeof(void*) * 100);
	auto data4 = pa->allocate(sizeof(void*) * 100);
	pa->deallocate(data2);
	pa->deallocate(data4);
	auto data5 = pa->allocate(sizeof(void*) * 100);
	assert(data4 == data5);
	auto data6 = pa->allocate(sizeof(void*) * 101);
	assert(data2 == data6);
	auto data7 = pa->allocate(sizeof(void*) * 110);
}
