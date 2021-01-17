#include <chrono>
#include <memory>

#include "artemis_game.hpp"
#include "platform.hpp"
#include "artemis_memory.hpp"
#include "renderer.hpp"
#include "artemis_camera.hpp"
#include "ui/gui.hpp"

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

	//platform::initializeInput(window);

	std::shared_ptr<platform::Window> window = platform::createWindow();
	auto renderer = std::shared_ptr<graphics::Renderer>(
		new graphics::Renderer(window.get())
	);
	//TODO? why is gui the only unique?
	auto gui = std::unique_ptr<ui::GUI>(new ui::GUI(window, renderer));

	IArena* arena = new StupidArena();

	Camera* camera = Camera::newCamera(
		arena, 
		glm::vec3(0.0f, 0.0f, 4.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	auto lastTime = std::chrono::high_resolution_clock::now();

	while(!platform::shouldCloseWindow(window.get()) && run) {
		stackArena->reset();	
		platform::pollEvents();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = 
			std::chrono::duration<float, std::chrono::seconds::period>(
				lastTime - currentTime
			).count();
		lastTime = currentTime;

		//just a place holder until i know what im doing
		renderer->newFrame();
		gui->newFrame();

		auto ubo = constructUBO(stackArena, camera);	
		renderer->drawFrame(ubo);	
	}
}
