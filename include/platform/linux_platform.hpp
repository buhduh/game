#ifndef LINUX_PLATFORM
#define LINUX_PLATFORM

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef GLFWwindow Window;

Window* createWindow();
bool shouldCloseWindow(Window*);
void destroyWindow(Window*);
void pollEvents();

#endif
