#include <iostream>
struct PtrStruct {
	int foo;
	float bar;
	void *start;
};

struct StaticStruct {
	int foo;
	float bar;
	uintptr_t allocations[1024];
};

int main(void) {
	std::cout << "PtrStruct: " << sizeof(PtrStruct) << std::endl;
	std::cout << "StaticStruct: " << sizeof(StaticStruct) << std::endl;
}
