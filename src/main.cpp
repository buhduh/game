#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_memory.hpp"
#include "artemis_asset.hpp"
#include "renderer.hpp"

#include "artemis_input.hpp"

void cbPressed1(input::Context* context, input::key_t key, input::event_t event) {
	STD_LOG("pressed 1");
}

void cbPressed2(input::Context* context, input::key_t key, input::event_t event) {
	STD_LOG("pressed 2");
}

void cbReleased(input::Context* context, input::key_t key, input::event_t event) {
	STD_LOG("released");
}

int main(void) {
	platform::Window* window = platform::createWindow();
	platform::initializeInput(window);
	IArena* arena = new StupidArena();
	MeshOrganizer mOrganizer(arena);
	Mesh mesh1 = getMeshAsset("Cube", &mOrganizer);
	renderer::Vulkan vulkan = renderer::Vulkan(window, 1, &mesh1);

	input::initializeInputSystem(arena, window);
	input::Context* context = input::requestNewContext();
	input::enableContext(context);
	assert(input::addCallbackToContext(context, input::KEY_PRESSED, 38, cbPressed1));
	assert(input::addCallbackToContext(context, input::KEY_PRESSED, 38, cbPressed2));
	assert(input::addCallbackToContext(context, input::KEY_RELEASED, 38, cbReleased));
	assert(input::removeCallbackFromContext(context, input::KEY_PRESSED, 38, cbPressed2));

	while(!platform::shouldCloseWindow(window)) {
		vulkan.drawFrame();	
		platform::pollEvents();
	}
	platform::destroyWindow(window);
}
