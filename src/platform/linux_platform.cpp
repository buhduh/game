#include <stdlib.h>

#include "platform/linux_platform.hpp"

Window* createWindow() {
	glfwInit();
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	return glfwCreateWindow(1024, 768, "game", 0, 0);
}

bool shouldCloseWindow(Window *window) {
	return !!glfwWindowShouldClose(window);
}

void destroyWindow(Window *window) {
	glfwDestroyWindow(window);
}

void pollEvents() {
	glfwPollEvents();
}

void *allocatePlatformMemory(size_t numBytes) {
	return malloc(numBytes);
}
