#include <vector>
#include <string.h>

#include "platform.hpp"
#include "artemis_asset.hpp"

std::string getMeshFileName(std::string meshName) {
	char nameBuffer[MAX_MESH_NAME];
	sprintf(nameBuffer, "%s/%s.bin", MESH_ASSET_DIR.c_str(), meshName.c_str());
	return std::string(nameBuffer);
}

bool writeMeshToFile(Mesh* mesh, std::string meshName) {
	std::string meshFileName = getMeshFileName(meshName);

	FILE* oFile = fopen(meshFileName.c_str(), "wb+");
	if(oFile == nullptr) {
		STD_ERR("Could not open outFile for writing: " << meshFileName);
		assert(false);
		return false;
	}
	auto numWrite = fwrite(&mesh->numVerts, sizeof(meshint_t), 1, oFile);
	assert(numWrite == 1);
	if(numWrite != 1) {
		fclose(oFile);
		return false;
	}

	numWrite = fwrite(&mesh->numNormals, sizeof(meshint_t), 1, oFile);
	assert(numWrite == 1);
	if(numWrite != 1) {
		fclose(oFile);
		return false;
	}

	numWrite = fwrite(&mesh->numIndeces, sizeof(meshint_t), 1, oFile);
	assert(numWrite == 1);
	if(numWrite != 1) {
		fclose(oFile);
		return false;
	}

	numWrite = fwrite(mesh->vertices, sizeof(vertex_t), mesh->numVerts, oFile);
	assert(numWrite == mesh->numVerts);
	if(numWrite != mesh->numVerts) {
		fclose(oFile);
		return false;
	}

	numWrite = fwrite(mesh->normals, sizeof(normal_t), mesh->numNormals, oFile);
	assert(numWrite == mesh->numNormals);
	if(numWrite != mesh->numNormals) {
		fclose(oFile);
		return false;
	}

	numWrite = fwrite(mesh->indeces, sizeof(index_t), mesh->numIndeces, oFile);
	assert(numWrite == mesh->numIndeces);
	if(numWrite != mesh->numIndeces) {
		fclose(oFile);
		return false;
	}

	fclose(oFile);
	return true;
}

Mesh* loadMeshFromFile(MeshMemoryManager* mManager, std::string meshName) {
	std::string meshFileName = getMeshFileName(meshName);	
	FILE* iFile = fopen(meshFileName.c_str(), "rb");
	if(iFile == nullptr) {
		STD_ERR("Could not open in file for reading: " << meshFileName);
		assert(false);
		return nullptr;
	}

	meshint_t numVerts;
	meshint_t numNormals;
	meshint_t numIndeces;

	auto numRead = fread(&numVerts, sizeof(meshint_t), 1, iFile);
	assert(numRead == 1);
	if(numRead != 1) {
		fclose(iFile);
		return nullptr;
	}

	numRead = fread(&numNormals, sizeof(meshint_t), 1, iFile);
	assert(numRead == 1);
	if(numRead != 1) {
		fclose(iFile);
		return nullptr;
	}

	numRead = fread(&numIndeces, sizeof(meshint_t), 1, iFile);
	assert(numRead == 1);
	if(numRead != 1) {
		fclose(iFile);
		return nullptr;
	}

	vertex_t verts[numVerts];
	normal_t normals[numNormals];
	index_t indeces[numIndeces];

	numRead = fread(verts, sizeof(vertex_t), numVerts, iFile);
	assert(numRead == numVerts);
	if(numRead != numVerts) {
		fclose(iFile);
		return nullptr;
	}

	numRead = fread(normals, sizeof(normal_t), numNormals, iFile);
	assert(numRead == numNormals);
	if(numRead != numNormals) {
		fclose(iFile);
		return nullptr;
	}

	numRead = fread(indeces, sizeof(index_t), numIndeces, iFile);
	assert(numRead == numIndeces);
	if(numRead != numIndeces) {
		fclose(iFile);
		return nullptr;
	}
	
	fclose(iFile);
	Mesh* toRet = mManager->newMesh(
		numVerts, numNormals, numIndeces, 
		verts, normals,indeces
	);

	assert(toRet);
	return toRet;
}
