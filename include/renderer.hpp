#ifndef RENDERER_HPP
#define RENDERER_HPP

//only vulkan for now
#include "renderer/vulkan.hpp"

namespace renderer {
	//don't know if this is applicable to other rendering backends
	//besides vulkan, going to assume it is
	size_t loadSpirV(char*, size_t, const std::string&);
	//adds up the verteces and sizeof vertex for all meshes in the list
	size_t getRequiredVertexBufferSizeFromMeshes(const meshcount_t, const Mesh*);
	size_t getRequiredIndexBufferSizeFromMeshes(const meshcount_t, const Mesh*);
	uint32_t getIndexCountFromMeshes(const meshcount_t, const Mesh*);
};

#endif
