#include "artemis_mesh.hpp"

//derefences the lists and returns ALL memory consumed
size_t getSizeOfMesh(Mesh* mesh) {
	size_t numVerts = static_cast<size_t>(mesh->numVerts);
	size_t vertSize = sizeof(vertex_t) * numVerts;
	size_t numIndeces = static_cast<size_t>(mesh->numIndeces);
	size_t indexSize = sizeof(vertexindex_t) * numIndeces;
	return numVerts + vertSize + numIndeces + indexSize;
}
