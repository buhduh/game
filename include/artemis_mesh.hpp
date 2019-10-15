#ifndef ARTEMIS_MESH_HPP
#define ARTEMIS_MESH_HPP

#include <glm/glm.hpp>

#include "artemis_game.hpp"

//how much memory to allocate to the mesh storage
//may end up ejecting these too
#define MESH_MEMORY megabytes(32)

//these must match, change them concurrently
#define MAX_VERTS UINT16_MAX
typedef uint16_t vertexindex_t ;

typedef glm::vec3 vertex_t;

//numVerts and numIndeces is a bit redundant, as this is saved
//in the header as well, 
//simplifies looking up the vertex and index lists though
struct Mesh {
	vertexindex_t numVerts;
	vertex_t* vertices;
	vertexindex_t numIndeces;
	vertexindex_t* indeces;  
};

struct MeshHeader {
	vertexindex_t numVerts;
	vertexindex_t numIndeces;
	//is an offset when in a file relative to the end of the MeshHeader* structure
	//or a pointer directly into the Mesh* structure when in memory
	union {
		uintptr_t offset, meshPtr;
	};
	char name[MAX_ASSET_NAME];
};

size_t getSizeOfMesh(Mesh*);

#endif
