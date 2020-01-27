#ifndef ARTEMIS_MESH_HPP
#define ARTEMIS_MESH_HPP

#include <iostream>
#include <glm/glm.hpp>

#include "artemis_game.hpp"
#include "artemis_memory.hpp"

#if 0
#define MAX_MESH_BUFFER_SZ UINT16_MAX
#define MAX_MESH_NAME UINT8_MAX
#define VERTEX_BUFFER_SIZE megabytes(32)
#define INDEX_BUFFER_SIZE megabytes(32)
#endif

const static uint8_t MAX_MESH_NAME = UINT8_MAX;
/*
	The maximum number of elements a mesh can have for its 
	respective buffers.
*/
typedef uint16_t meshint_t;
const static meshint_t MESHINT_MAX UINT16_MAX;
/*
	The maximum number of meshes a MeshMemoryManager
	will manage.
*/
const static meshint_t MAX_MESH_COUNT = UINT16_MAX;

//x is verts, y is textures, z is normals
typedef glm::uvec3 index_t;
typedef glm::vec3 vertex_t;
typedef glm::vec3 normal_t;
//these buffer_t's are being cast to and from void*
//be careful if their types change
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
	//accessors are kinda dumb, 
	//not sure if I should make this public or private...
	meshint_t meshCount;	
	private:
	GameMemory* gameMemory;
	IArena* vertArena;
	IArena* normalArena;
	IArena* indexArena;
	IArena* meshArena;
};

#endif
