#include <vector>
#include <string.h>

#include "platform.hpp"
#include "artemis_asset.hpp"

Mesh getMeshAsset(const std::string name, MeshMemoryManager* mMemManager) {
	if(MeshAssets.count(name)) {
		return MeshAssets[name];
	}
	Mesh mesh = loadMeshFromAssetDir(name, mMemManager);
	assert(!mesh.isNil());
	return mesh;
}

//TODO verify meshes don't blow past allocated memory
//TODO gonna need to rethink this once normals are in place
Mesh loadMeshFromAssetDir(const std::string name, MeshMemoryManager* mMemManager) {
	return Mesh();
}
/*
Mesh loadMeshFromAssetDir(const std::string name, MeshOrganizer* mOrganizer) {
	std::vector<std::string> files = platform::getFilesInDir(std::string(MESH_ASSET_DIR));
	MeshFileHeader mFileHeader;
	size_t readCount;
	for(auto& f: files) {
		FILE* aFile = fopen(f.c_str(), "rb");
		assert(aFile);
		readCount = fread(&mFileHeader, sizeof(MeshFileHeader), 1, aFile);
		assert(readCount == 1);
		//this might be a problem if the MeshHeader list is too long
		MeshHeader mHeaders[mFileHeader.numMeshes];
		readCount =
			fread(mHeaders, sizeof(MeshHeader), mFileHeader.numMeshes, aFile);
		assert(readCount == mFileHeader.numMeshes);
		for(meshcount_t i = 0; i < mFileHeader.numMeshes; i++) {
			if(strcmp(name.c_str(), mHeaders[i].name) != 0) continue;
			assert(fseek(aFile, mHeaders[i].offset, SEEK_CUR) == 0);
			readCount = fread(
				mOrganizer->vertexBufferEnd, sizeof(vertex_t), 
				(size_t) mHeaders[i].numVerts, aFile
			);
			assert(readCount == mHeaders[i].numVerts);
			readCount = fread(
				mOrganizer->indexBufferEnd, sizeof(vertexindex_t),
				mHeaders[i].numIndeces, aFile
			);
			assert(readCount == mHeaders[i].numIndeces);
			Mesh toRet = Mesh(
				mHeaders[i].numVerts, mHeaders[i].numIndeces,
				mOrganizer->vertexBufferEnd, mOrganizer->indexBufferEnd
			);
			mOrganizer->vertexBufferEnd += mHeaders[i].numVerts;
			mOrganizer->indexBufferEnd += mHeaders[i].numIndeces;
			MeshAssets[name] = toRet;
			return toRet;
		}
	}
	return Mesh();
}
*/

std::ostream& operator<<(std::ostream& out, const meshassets_t& mAssets) {
	for(auto i = mAssets.begin(); i != mAssets.end(); ++i) {
		out << "asset name: " << i->first << std::endl;	
		out << i->second;
	}
	return out;
}
