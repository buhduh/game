#ifndef LINUX_PLATFORM
#define LINUX_PLATFORM

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace platform {
	typedef GLFWwindow Window;
	typedef int input_key_t;
	typedef int input_event_t;
	const input_key_t KEY_UNASSIGNED = -1;
}

#endif
