#ifndef ARTEMIS_ASSET_HPP
#define ARTEMIS_ASSET_HPP

#include <string>
#include <map>

#include "artemis_memory.hpp"
#include "artemis_mesh.hpp"

//When this fills up will begin ejecting older meshes that
//aren't in use
//these must match, change them concurrently
#define MAX_CONCURRENT_MESHES UINT16_MAX
typedef uint16_t meshcount_t;

#define MESH_ASSET_DIR "assets/meshes"

struct AssetManager {	
	IArena* arena;
	MeshHeader mHeader[MAX_CONCURRENT_MESHES];
	Mesh* meshes;
	meshcount_t numMeshes;
};

struct MeshFileHeader {
	meshcount_t numMeshes;
};

AssetManager* newAssetManager(IArena*);
Mesh* getMeshAsset(std::string);
Mesh* loadMeshFromAssetDirectory(std::string, AssetManager*);

#endif
