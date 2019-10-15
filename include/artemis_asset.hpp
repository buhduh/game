#ifndef ARTEMIS_ASSET_HPP
#define ARTEMIS_ASSET_HPP

#include <string>
#include <map>

#include "artemis_memory.hpp"
#include "artemis_entity.hpp"

#define MAX_ASSET_NAME 64

struct AssetManager {	
	IArena* arena;
};

struct MeshHeader {
	int numVerts;
	int numIndeces;
	uintptr_t offset;
	char name[MAX_ASSET_NAME];
};

struct MeshFileHeader {
	int numMeshes;
};

#endif
