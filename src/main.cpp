#include "platform.hpp"
#include "game.hpp"
#include "memory.hpp"

int main(void) {
	GameMemory persistentStorage = newGameMemory(kilobyte(1));
	Window *window = createWindow();
	while(!shouldCloseWindow(window)) {
		pollEvents();
	}
	destroyWindow(window);
}
