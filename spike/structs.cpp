#include "artemis_game.hpp"

int main() {
	const long long zero = megabytes(0);
	const long long one = megabytes(1);
	const long long two = megabytes(2);
	const long long three = megabytes(3);
	struct{
		uint8_t i[zero];
	} a;
	struct{
		uint8_t i[one];
	} b;
	struct{
		uint8_t i[two];
	} c;
	struct{
		uint8_t i[three];
	} d;

	STD_LOG("a: " << sizeof(a));
	STD_LOG("b: " << sizeof(b));
	STD_LOG("c: " << sizeof(c));
	STD_LOG("d: " << sizeof(d));
}
