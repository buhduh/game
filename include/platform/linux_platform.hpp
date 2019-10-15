#ifndef LINUX_PLATFORM
#define LINUX_PLAtTFORM

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef GLFWwindow Window;

Window* createWindow();
bool shouldCloseWindow(Window*);
void destroyWindow(Window*);
void pollEvents();
void* allocatePlatformMemory(size_t);
void deallocatePlatformMemory(void*);

#endif
