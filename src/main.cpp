#include <chrono>
#include <memory>

#include "artemis_game.hpp"
#include "platform.hpp"
#include "artemis_memory.hpp"
#include "renderer.hpp"
#include "artemis_camera.hpp"

#include "imgui/imgui.h"

static bool run = true;

graphics::UniformBufferObject* constructUBO(IArena* arena, Camera* camera) {
	graphics::UniformBufferObject ubo = {};
	
	//auto xRotation = glm::rotate(glm::mat4(1.0f), 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//auto yRotation = glm::rotate(glm::mat4(1.0f), 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	auto zRotation = glm::rotate(glm::mat4(1.0f), 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	
	//ubo.model = zRotation*yRotation*xRotation;
	//ubo.model = glm::mat4(1.0f);
	ubo.model = zRotation;
	ubo.view = camera->view;
	graphics::UniformBufferObject* toRet = 
		(graphics::UniformBufferObject*) arena->allocate(sizeof(ubo));
	*toRet = ubo;
	return toRet;
}

int main(void) {
	GameMemory* mainMemory = new GameMemory(GIGABYTES(1));
	StackArena* stackArena = 
		mainMemory->newStackArena(KILOBYTES(1));

	platform::Window* window = platform::createWindow();
	platform::initializeInput(window);

	//graphics::Vulkan vulkan = graphics::Vulkan(window);
	//std::unique_ptr<graphics::Renderer<DEF_RENDERER>> renderer = graphics::makeDefaultRenderer(window);
	std::unique_ptr<graphics::Renderer<graphics::Vulkan>> renderer = graphics::makeDefaultRenderer(window);

	IArena* arena = new StupidArena();

	Camera* camera = Camera::newCamera(
		arena, 
		glm::vec3(0.0f, 0.0f, 4.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	auto lastTime = std::chrono::high_resolution_clock::now();

	while(!platform::shouldCloseWindow(window) && run) {
		stackArena->reset();	
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = 
			std::chrono::duration<float, std::chrono::seconds::period>(
				lastTime - currentTime
			).count();
		lastTime = currentTime;
		platform::pollEvents();

		auto ubo = constructUBO(stackArena, camera);	
		//vulkan.drawFrame(ubo);	
	}
	platform::destroyWindow(window);
}
