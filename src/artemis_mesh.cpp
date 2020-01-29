#include <cstring>

#include "artemis_mesh.hpp"

Mesh::Mesh(
	meshint_t numVerts, 
	meshint_t numNormals, 
	meshint_t numIndeces, 
	vertexbuffer_t verts, 
	normalbuffer_t normals,
	indexbuffer_t indeces
) 
	: numVerts(numVerts)
	, numNormals(numNormals)
	, numIndeces(numIndeces)
	, vertices(verts)
	, normals(normals)
	, indeces(indeces)
{}

Mesh::Mesh() 
	: numVerts(0)
	, numNormals(0)
	, numIndeces(0)
	, vertices(nullptr)
	, normals(nullptr)
	, indeces(nullptr) 
{}


//derefences the lists and returns ALL memory consumed
//not sure if i even need this...
/*
size_t Mesh::getSizeOf() {
	size_t numVertsSize = sizeof(numVerts);
	size_t vertSize = sizeof(vertex_t) * numVerts;
	size_t numIndecesSize = sizeof(numIndeces);
	size_t indexSize = sizeof(vertexindex_t) * numIndeces;
	return numVertsSize + vertSize + numIndecesSize + indexSize;
}
*/

MeshMemoryManager::MeshMemoryManager(
	IArena* vertArena,
	IArena* normalArena,
	IArena* indexArena,
	IArena* meshArena
) : vertArena(vertArena)
  , normalArena(normalArena)
  , indexArena(indexArena)
  , meshArena(meshArena)
  , gameMemory(nullptr)
  , meshCount(0)
{}

Mesh* MeshMemoryManager::newMesh(
	meshint_t numVerts, 
	meshint_t numNormals,
	meshint_t numIndeces, 
	vertexbuffer_t verts, 
	normalbuffer_t normals,
	indexbuffer_t indeces
)
{
	vertexbuffer_t vertBuffer = 
		(vertexbuffer_t) vertArena->allocate(sizeof(vertex_t) * numVerts);
	normalbuffer_t normalBuffer = 
		(normalbuffer_t) normalArena->allocate(sizeof(normal_t) * numNormals);
	indexbuffer_t indexBuffer = 
		(indexbuffer_t) indexArena->allocate(sizeof(index_t) * numIndeces);
	//is verts nil on failure?
	memcpy(vertBuffer, verts, sizeof(vertex_t) * numVerts);
	memcpy(normalBuffer, normals, sizeof(normal_t) * numNormals);
	memcpy(indexBuffer, indeces, sizeof(index_t) * numIndeces);
	Mesh* mesh = (Mesh*) meshArena->allocate(sizeof(Mesh));
	assert(mesh);
	*mesh = Mesh(
		numVerts, numNormals, numIndeces, 
		vertBuffer, normalBuffer, indexBuffer
	);
	meshCount++;
	return mesh;
}

//TODO will need to probably use better arenas
MeshMemoryManager::MeshMemoryManager(GameMemory* gMemory) 
  : gameMemory(gMemory)
  , meshCount(0)
{	
	//this absolutely leaks, but i need a memory solution
	vertArena = new StupidArena();
	normalArena = new StupidArena();
	indexArena = new StupidArena();
	meshArena = new StupidArena();
#if 0
	vertArena = gameMemory->newStackArena(
		sizeof(vertex_t) * MAX_MESH_BUFFER_SZ * MAX_MESH_COUNT
	);
	normalArena = gameMemory->newStackArena(
		sizeof(normal_t) * MAX_MESH_BUFFER_SZ * MAX_MESH_COUNT
	);
	indexArena = gameMemory->newStackArena(
		sizeof(index_t) * MAX_MESH_BUFFER_SZ * MAX_MESH_COUNT
	);
	meshArena = gameMemory->newStackArena(
		sizeof(Mesh) * MAX_MESH_COUNT
	);
#endif
}

//don't deallocate the arenas if the memory manager didn't make them
MeshMemoryManager::~MeshMemoryManager() {
	if(!gameMemory) return;
	gameMemory->deallocateArena(vertArena);
	gameMemory->deallocateArena(normalArena);
	gameMemory->deallocateArena(indexArena);
	gameMemory->deallocateArena(meshArena);
}

bool Mesh::isNil() {
	return numVerts == 0;
}

std::ostream& operator<<(std::ostream &out, const Mesh &mesh) {
	out << "vertex count: " << mesh.numVerts << std::endl;
	out << "normal count: " << mesh.numNormals << std::endl;
	out << "index count: " << mesh.numIndeces << std::endl;
	out << "vertex buffer ptr: " << mesh.vertices << std::endl;
	out << "normal buffer ptr: " << mesh.normals << std::endl;
	out << "index buffer ptr: " << mesh.indeces << std::endl;
	if(mesh.numVerts == 0) {
		out << "vertex list: nil" << std::endl;
	} else {
		out << "vertex list:" << std::endl;
		for(meshint_t i = 0; i < mesh.numVerts; i++) {
			out << "\tx: " << mesh.vertices[i].x << " y: " << mesh.vertices[i].y
			<< " z: " << mesh.vertices[i].z << std::endl;
		}
	}
	if(mesh.numNormals == 0) {
		out << "normal list: nil" << std::endl;
	} else {
		out << "normal list:" << std::endl;
		for(meshint_t i = 0; i < mesh.numNormals; i++) {
			out << "\tx: " << mesh.normals[i].x << " y: " << mesh.normals[i].y
			<< " z: " << mesh.normals[i].z << std::endl;
		}
	}
	if(mesh.numIndeces == 0) {
		out << "index list: nil" << std::endl;
	} else {
		out << "index list:" << std::endl;
		for(meshint_t i = 0; i < mesh.numIndeces; i++) {
			out << "\tv index: " << mesh.indeces[i].x << " n/a: " << mesh.indeces[i].y
			<< " n index: " << mesh.indeces[i].z << std::endl;
		}
	}
	return out;
}
