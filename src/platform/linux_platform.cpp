#include <filesystem>
#include <fstream>

#include "platform.hpp"
#include "artemis_game.hpp"
#include "artemis_input.hpp"

namespace platform {

	static const size_t DEFAULT_CACHE_LINE_SIZE = 64;
	static size_t setCacheLineSize = 0;

	size_t getCacheLineSize() {
		if(setCacheLineSize > 0) {
			return setCacheLineSize;
		}
		std::string where(
			"/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size");
		FILE* p = fopen(where.c_str(), "r");
		if(!p) {
			setCacheLineSize = DEFAULT_CACHE_LINE_SIZE;
			return setCacheLineSize;
		}
		int i;
		fscanf(p, "%d", &i);
		fclose(p);
		setCacheLineSize = i;
		return setCacheLineSize;
	}

	void getWindowSize(Window* window, int& height, int& width) {
		glfwGetWindowSize(window, &width, &height);
	}

	std::shared_ptr<Window> createWindow() {
		glfwInit();
		auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		//return glfwCreateWindow(1024, 768, "game", 0, 0);
		//NOTE: comment the rest of this function and uncomment
		//the above line to change the window to the params above
		Window* window = glfwCreateWindow(
			mode->width, mode->height, 
			"game", monitor, 0
		);
		glfwSetWindowMonitor(
			window, nullptr, 0, 0, 
			mode->width, mode->height, 
			mode->refreshRate
		);
		return std::shared_ptr<Window>(window, destroyWindow);
	}

	void keyCallback(
		Window* window, int key, 
		int scancode, int action, int mods
	) {
		input::internalInputCallback(scancode, action);
	}

	void initializeInput(Window* window) {
		glfwSetKeyCallback(window, keyCallback);
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

	void* allocatePlatformMemory(size_t numBytes) {
		return malloc(numBytes);
	}

	void deallocatePlatformMemory(void *mem) {
		free(mem);
	}

	//this sucks, i don't care, don't use in a tight loop
	std::vector<std::string> getFilesInDir(std::string dir) {
		std::vector<std::string> toRet;
		for(auto& p: std::filesystem::directory_iterator(dir)) {
			toRet.push_back(p.path().string());
		}
		return toRet;
	}

	size_t getFileSize(const std::string& fName) {
		std::filesystem::path p(fName); 
		return std::filesystem::file_size(p);
	}

	size_t loadFileIntoBuffer(char* buffer, size_t size, const std::string& fName) {
		std::ifstream file(fName, std::ios::binary);
		assert(file.is_open());
		file.read(buffer, size);
		assert(file.tellg() == size);
		file.close();
		return size;
	}
}
