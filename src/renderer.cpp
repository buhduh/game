#include "renderer.hpp"

namespace renderer {
	size_t loadSpirV(char* buffer, size_t bufferSize, const std::string& fileName) {
		return platform::loadFileIntoBuffer(buffer, bufferSize, fileName);
	}

	size_t getRequiredVertexBufferSizeFromMeshes(
		const meshcount_t numMeshes, const Mesh* meshes
	) {
		size_t tot = 0;
		size_t vertexSize = sizeof(vertex_t);
		for(meshcount_t i = 0; i < numMeshes; ++i) {
			tot += (meshes[i].numVerts * vertexSize);
		}
		return tot;
	}
	size_t getRequiredIndexBufferSizeFromMeshes(
		const meshcount_t numMeshes, const Mesh* meshes
	) {
		size_t tot = 0;
		size_t indexSize = sizeof(index_t);
		for(meshcount_t i = 0; i < numMeshes; ++i) {
			tot += (meshes[i].numIndeces * indexSize);
		}
		return tot;
	}

	//can't imagine there being 4 billion of these...
	uint32_t getIndexCountFromMeshes(const meshcount_t numMeshes, const Mesh* meshes) {
		uint32_t tot = 0;
		for(meshcount_t i = 0; i < numMeshes; i++) {
			tot += (meshes[i].numIndeces);
		}
		return tot;
	}
};
