#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "renderer.hpp"

int main(void) {
	platform::Window* window = platform::createWindow();
	renderer::Vulkan Vulkan = renderer::Vulkan(window);
	while(!platform::shouldCloseWindow(window)) {
		platform::pollEvents();
	}
	platform::destroyWindow(window);
}
