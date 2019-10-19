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

//TODO verify meshes don't blow past allocated memory
MeshOrganizer::MeshOrganizer(IArena* _arena) :
	arena(_arena)
{
	vertexBuffer = (vertexbuffer_t) arena->allocate(VERTEX_BUFFER_SIZE);
	indexBuffer = (indexbuffer_t) arena->allocate(INDEX_BUFFER_SIZE);
	vertexBufferEnd = vertexBuffer;
	indexBufferEnd = indexBuffer;
}

bool Mesh::isNil() {
	return numVerts == 0;
}

std::ostream& operator<<(std::ostream &out, const Mesh &mesh) {
	out << "vertex count: " << mesh.numVerts << std::endl;
	out << "index count: " << mesh.numIndeces << std::endl;
	out << "vertex buffer ptr: " << mesh.vertices << std::endl;
	out << "index buffer ptr: " << mesh.indeces << std::endl;
	if(mesh.numVerts == 0) {
		out << "vertex list: nil" << std::endl;
	} else {
		out << "vertex list:" << std::endl;
		for(vertexindex_t i = 0; i < mesh.numVerts; i++) {
			out << "\tx: " << mesh.vertices[i].x << " y: " << mesh.vertices[i].y
			<< " z: " << mesh.vertices[i].y << std::endl;
		}
	}
	//these should have multiples of 3...
	if(mesh.numIndeces == 0) {
		out << "index list: nil" << std::endl;
	} else {
		out << "index list:" << std::endl;
		for(vertexindex_t i = 0; i < mesh.numVerts;) {
			out << "\t" << mesh.indeces[i++] << ", " << mesh.indeces[i++]
			<< ", " << mesh.indeces[i++] << std::endl;
		}
	}
	return out;
}
