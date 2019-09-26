#include "platform.hpp"

int main(void) {
	Window *window = createWindow();
	while(!shouldCloseWindow(window)) {
		pollEvents();
	}
	destroyWindow(window);
}
