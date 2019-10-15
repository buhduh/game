#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"

int main(void) {
	platform::Window *window = platform::createWindow();
	while(!platform::shouldCloseWindow(window)) {
		platform::pollEvents();
	}
	platform::destroyWindow(window);
}
