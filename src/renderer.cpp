#include "renderer.hpp"

namespace renderer {
	size_t loadSpirV(char* buffer, size_t bufferSize, const std::string& fileName) {
		return platform::loadFileIntoBuffer(buffer, bufferSize, fileName);
	}
};
