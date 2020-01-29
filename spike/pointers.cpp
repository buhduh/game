#include "artemis_game.hpp"

int main() {
	int* foo;
	int* bar;
	int baz = 5;
	foo = &baz;
	bar = &baz;
	*foo = 42;
	STD_LOG("foo: " << *foo << ", bar: " << *bar << ", baz: " << baz);
}
