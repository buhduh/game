#include "artemis_mesh.hpp"

Mesh::Mesh(
	vertexindex_t numVerts, vertexindex_t numIndeces, 
	vertex_t* verts, vertexindex_t* indeces) :
	numVerts(numVerts), numIndeces(numIndeces),
	vertices(verts), indeces(indeces)
{}

Mesh::Mesh() : numVerts(0), numIndeces(0), vertices(nullptr), indeces(nullptr) {}

//derefences the lists and returns ALL memory consumed
size_t Mesh::getSizeOf() {
	size_t numVertsSize = sizeof(numVerts);
	size_t vertSize = sizeof(vertex_t) * numVerts;
	size_t numIndecesSize = sizeof(numIndeces);
	size_t indexSize = sizeof(vertexindex_t) * numIndeces;
	return numVertsSize + vertSize + numIndecesSize + indexSize;
}

MeshOrganizer::MeshOrganizer(IArena* _arena) :
	arena(_arena)
{
	vertexBuffer = (vertexbuffer_t) arena->allocate(VERTEX_BUFFER_SIZE);
	indexBuffer = (indexbuffer_t) arena->allocate(INDEX_BUFFER_SIZE);
	vertexBufferEnd = vertexBuffer;
	indexBufferEnd = indexBuffer;
}

bool Mesh::isNil() {
	return numVerts;	
}
