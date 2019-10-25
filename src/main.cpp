#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "artemis_asset.hpp"
#include "renderer.hpp"

int main(void) {
	platform::Window* window = platform::createWindow();
	platform::initializeInput(window);
	IArena* arena = new StupidArena();
	MeshOrganizer mOrganizer(arena);
	Mesh mesh1 = getMeshAsset("Cube", &mOrganizer);
	renderer::Vulkan vulkan = renderer::Vulkan(window, 1, &mesh1);
	while(!platform::shouldCloseWindow(window)) {
		vulkan.drawFrame();	
		platform::pollEvents();
	}
	platform::destroyWindow(window);
}
