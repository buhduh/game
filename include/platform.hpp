/*
platform agnostic "simple include"
intended for a simple preprocessor if switch
*/
#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <vector>
#include <string>
#include <stdlib.h>

#ifdef __linux__
#include "platform/linux_platform.hpp"
#endif

namespace platform {
	Window* createWindow();
	bool shouldCloseWindow(Window*);
	void destroyWindow(Window*);
	void pollEvents();
	void* allocatePlatformMemory(size_t);
	void deallocatePlatformMemory(void*);
	std::vector<std::string> getFilesInDir(std::string);
	size_t getFileSize(const std::string&);
	size_t loadFileIntoBuffer(char*, size_t, const std::string&);
	void initializeInput(Window*);
	size_t getCacheLineSize();
	void getWindowSize(Window*, int& height, int& width);
}

#endif
