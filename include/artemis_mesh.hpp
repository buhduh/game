#ifndef ARTEMIS_MESH_HPP
#define ARTEMIS_MESH_HPP

#include <iostream>
#include <glm/glm.hpp>

#include "artemis_game.hpp"
#include "artemis_memory.hpp"

//how much memory to allocate to mesh data
//may end up ejecting these too
#define VERTEX_BUFFER_SIZE megabytes(32)
#define INDEX_BUFFER_SIZE megabytes(32)

#define MAX_MESH_NAME UINT8_MAX

//these must match, change them concurrently
#define MAX_VERTS UINT16_MAX
typedef uint16_t vertexindex_t ;
typedef glm::vec3 vertex_t;
typedef vertex_t* vertexbuffer_t;
typedef vertexindex_t* indexbuffer_t;

//for now the buffers are pointers into the MeshOrganizer
class Mesh {
	public:
	Mesh(vertexindex_t, vertexindex_t, vertex_t*, vertexindex_t*);
	Mesh();
	size_t getSizeOf();
	vertexindex_t numVerts;
	vertexindex_t numIndeces;
	vertexbuffer_t vertices;
	indexbuffer_t indeces;  
	bool isNil();
	friend std::ostream& operator<<(std::ostream&, const Mesh&);
};

//Memory organization stuff goes here
//TODO don't blow past buffer sizes when adding meshes
//the renderer will blow up if vertex list
//isn't contiguous in memory!!!
class MeshOrganizer {
	public:
	MeshOrganizer(IArena*);
	vertex_t* vertexBufferEnd;
	vertexindex_t* indexBufferEnd;
	vertexbuffer_t vertexBuffer;
	indexbuffer_t indexBuffer;
	private:
	IArena* arena;
};

size_t getSizeOfVertexBuffer(Mesh*, meshcount_t);

#endif
