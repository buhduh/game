#include "artemis_game.hpp"

void doFoo(int*& foo) {
	STD_LOG(foo);
}

int main() {
	int foo = 42;
	int* fooP = &foo;
	doFoo(fooP);
	//doFoo(&foo);
}
