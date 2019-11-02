#include "artemis_memory.hpp"

struct huge {
	uintptr_t foo[100];
};

int main() {
	GameMemory* gm = new GameMemory(gigabytes(1));
	auto pa = gm->newConstantPoolArena(3, 100, sizeof(huge));
	auto data1 = pa->allocate(sizeof(huge) * 100);
	auto data2 = pa->allocate(sizeof(huge) * 100);
	auto data3 = pa->allocate(sizeof(huge) * 100);
	pa->deallocate(data1);
	pa->deallocate(data3);
	auto data4 = pa->allocate(sizeof(huge) * 100);
	assert(data4 == data3);
	auto data5 = pa->allocate(sizeof(huge) * 100);
	assert(data5 == data1);
	auto data6 = pa->allocate(sizeof(huge) * 100);
}
