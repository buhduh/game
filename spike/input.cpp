#include "artemis_input.hpp"
#include "artemis_memory.hpp"

int main(void) {
	IArena* arena = new StupidArena();
	input::initializeInputSystem(arena, nullptr);
	input::Context* context = input::requestNewContext();
	input::enableContext(context);
	assert(input::addCallbackToContext(context, input::KEY_PRESSED, 8, cb));
}
