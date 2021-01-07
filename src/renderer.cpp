#include "renderer.hpp"

using namespace graphics;

/*
size_t loadSpirV(char* buffer, size_t bufferSize, const std::string& fileName) {
	return platform::loadFileIntoBuffer(buffer, bufferSize, fileName);
}
*/

std::unique_ptr<Renderer<Vulkan>> graphics::makeDefaultRenderer(platform::Window* window) {
	return std::unique_ptr<Renderer<Vulkan>>(new Renderer<Vulkan>(window));
}
