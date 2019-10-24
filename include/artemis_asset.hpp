#ifndef ARTEMIS_ASSET_HPP
#define ARTEMIS_ASSET_HPP

#include <unordered_map>

#include "artemis_mesh.hpp"

#define MESH_ASSET_DIR "assets/meshes"

//This may become a problem...
typedef std::unordered_map<std::string, Mesh> meshassets_t;
static meshassets_t MeshAssets = meshassets_t(MAX_CONCURRENT_MESHES);

struct MeshFileHeader {
	meshcount_t numMeshes;
};

struct MeshHeader {
	vertexindex_t numVerts;
	vertexindex_t numIndeces;
	//where the mesh is in the asset file relative to the end
	//of the MeshHeader list.
	uintptr_t offset;
	char name[MAX_MESH_NAME];
};

Mesh getMeshAsset(const std::string, MeshOrganizer*);
Mesh loadMeshFromAssetDir(const std::string, MeshOrganizer*);

#endif
