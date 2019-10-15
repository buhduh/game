#include <vector>
#include <string.h>

#include "artemis_asset.hpp"
#include "artemis_game.hpp"
#include "platform.hpp"

//TODO will need to bulk load assets at somepoint probably

AssetManager* newAssetManager(IArena* arena) {
	AssetManager* toRet = (AssetManager*) arena->allocate(sizeof(AssetManager));
	assert(toRet);
	Mesh* tMesh = (Mesh*) arena->allocate(MESH_MEMORY);
	assert(tMesh);
	toRet->meshes = tMesh;
	toRet->arena = arena;
	toRet->numMeshes = 0;
	return toRet;
}

Mesh* getMesh(std::string name, AssetManager* aManager) {
	for(meshcount_t i = 0; i < aManager->numMeshes; i++) {
		if(name == std::string(aManager->mHeader[i].name)) {
			return reinterpret_cast<Mesh*>(aManager->mHeader[i].meshPtr);
		}
	}
	Mesh* toRet = loadMeshFromAssetDirectory(name, aManager);
	assert(toRet);
	return toRet;
}

//use the c FILE interface, the c++ one is stupid and complicated, don't care
//is this safe to use outside of the platform layer?
//loads the mesh into the asset manager
//TODO this requires testing
Mesh* loadMeshFromAssetDirectory(std::string name, AssetManager* aManager) {
	//sooner or later this will blow up
	assert(aManager->numMeshes < MAX_CONCURRENT_MESHES);
	std::vector<std::string> assetFiles = platform::getFilesInDir(MESH_ASSET_DIR);
	MeshFileHeader mFileHeader;
	size_t mFSize = sizeof(MeshFileHeader);
	size_t mHSize = sizeof(MeshHeader);
	Mesh toRet;
	for(auto& f: assetFiles) {
		FILE* aFile = fopen(f.c_str(), "rb");
		assert(aFile);
		assert(fread(&mFileHeader, mFSize, 1, aFile) == mFSize);
		if(!mFileHeader.numMeshes) continue;
		//MeshHeader mHeaders[mFileHeader.numMeshes] = MeshHeader{0};
		MeshHeader mHeaders[mFileHeader.numMeshes];
		assert(
			fread(mHeaders, mHSize, mFileHeader.numMeshes, aFile) ==
			mHSize * mFileHeader.numMeshes
		);
		for(meshcount_t i = 0; i < mFileHeader.numMeshes; i++) {
			MeshHeader mHeader = mHeaders[i];	
			if(strcmp(mHeader.name, name.c_str())) {
				meshcount_t headerIndex = aManager->numMeshes;
				memcpy(
					&aManager->mHeader[headerIndex], 
					&mHeader, mHSize
				);
				uintptr_t mPtr = aManager->mHeader[headerIndex - 1].meshPtr;
				mPtr += static_cast<uintptr_t>(getSizeOfMesh((Mesh*) mPtr));
				aManager->mHeader[headerIndex].meshPtr = mPtr;
				Mesh* mesh = reinterpret_cast<Mesh*>(mPtr);
				fseek(aFile, mHeader.offset, SEEK_CUR);
				size_t tot = 0;
				tot += fread(&mesh->numVerts, sizeof(vertexindex_t), 1, aFile);
				tot += fread(mesh->vertices, sizeof(vertex_t), mesh->numVerts, aFile);
				tot += fread(&mesh->numIndeces, sizeof(vertexindex_t), 1, aFile);
				tot += fread(mesh->indeces, sizeof(vertexindex_t), mesh->numIndeces, aFile);
				fclose(aFile);
				assert(tot == getSizeOfMesh(mesh));
				aManager->numMeshes += 1;
				return mesh;
			}
		}
	}
	return nullptr;
}
