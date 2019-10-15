#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"

int main(void) {
	Window *window = createWindow();
	while(!shouldCloseWindow(window)) {
		pollEvents();
	}
	destroyWindow(window);
}
