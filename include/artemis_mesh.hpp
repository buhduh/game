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

#if 0
#define MAX_MESH_BUFFER_SZ UINT16_MAX
#define MAX_MESH_NAME UINT8_MAX
#endif

const static uint8_t MAX_MESH_NAME = UINT8_MAX;
/*
	The maximum number of elements a mesh can have for its 
	respective buffers.
*/
typedef uint16_t meshint_t;
const static meshint_t MAX_MESH_BUFFER_SZ = UINT16_MAX;
/*
	The maximum number of meshes a MeshMemoryManager
	will manage.
*/
const static meshint_t MAX_MESH_COUNT = UINT16_MAX;

typedef glm::uvec3 index_t;
typedef glm::vec3 vertex_t;
typedef glm::vec3 normal_t;
typedef vertex_t* vertexbuffer_t;
typedef index_t* indexbuffer_t;
typedef normal_t* normalbuffer_t;

//for now the buffers are pointers into the MeshOrganizer
class Mesh {
	public:
	Mesh(
		meshint_t numVerts, 
		meshint_t numNormals,
		meshint_t numIndeces, 
		vertexbuffer_t verts, 
		normalbuffer_t normals,
		indexbuffer_t indeces
	);
	Mesh();
	size_t getSizeOf();
	meshint_t numVerts;
	meshint_t numNormals;
	meshint_t numIndeces;
	vertexbuffer_t vertices;
	normalbuffer_t normals;
	indexbuffer_t indeces;  
	bool isNil();
	friend std::ostream& operator<<(std::ostream&, const Mesh&);
};

class MeshMemoryManager {
	public:
	MeshMemoryManager(
		IArena* vertArena,
		IArena* normalArena,
		IArena* indexArena,
		IArena* meshArena
	);
	MeshMemoryManager(GameMemory*);
	~MeshMemoryManager();
	Mesh* newMesh(
		meshint_t numVerts, 
		meshint_t numNormals,
		meshint_t numIndeces, 
		vertexbuffer_t verts, 
		normalbuffer_t normals,
		indexbuffer_t indeces
	);
	private:
	GameMemory* gameMemory;
	IArena* vertArena;
	IArena* normalArena;
	IArena* indexArena;
	IArena* meshArena;
	meshint_t meshCount;	
};

#endif
