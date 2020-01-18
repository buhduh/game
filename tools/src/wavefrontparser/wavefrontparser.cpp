#include "artemis_asset.hpp"
#include "wavefrontparser.hpp"

#include <cstdio>

ParsedArgs parseArgs(int argc, char* argv[]) {
	return ParsedArgs{
		inFile: std::string(argv[1]),
		outFile: std::string(argv[2]),
	};
}


bool checkInFile(std::string inFile) {
	std::ifstream f(inFile);
	return f.good();
}

void errorAndBail(std::string msg) {
	std::cerr << msg << std::endl;
	exit(EXIT_FAILURE);
}

TYPE getTypeFromString(std::string line) {
	auto type = TYPE_MAP.find(line.substr(0, 2));
	if(type == TYPE_MAP.end()) {
		return NONE;
	}
	return type->second;
}

bool processNormalLine(std::string line, NormalTracker* nTracker) {
	if(nTracker->nIndex == MAX_MESH_BUFFER_SZ) {
		return false;
	}
	if(!std::regex_match(line.c_str(), match, NORMAL_PATT)) {
		return false;
	}
	nTracker->nBuffer[nTracker->nIndex++] = vertex_t(
		atof(match.str(1).c_str()),
		atof(match.str(2).c_str()),
		atof(match.str(3).c_str())
	);
	STD_LOG("processed normal line: " << line);
	return true;
}

bool processFaceLine(std::string line, FaceTracker* fTracker) {
	STD_LOG("processed face line: " << line);
	if(!std::regex_match(line.c_str(), match, FACE_PATT)) {
		return false;
	}
	if(fTracker->fIndex + 3 > MAX_MESH_BUFFER_SZ) {
		return false;
	}
	//should always be multiples of 3, what happens if we end up doing different
	//topologies?
	fTracker->fBuffer[fTracker->fIndex++] = index_t(
		atoi(match.str(1).c_str()) - 1,
		0,
		atoi(match.str(2).c_str()) - 1
	);
	fTracker->fBuffer[fTracker->fIndex++] = index_t(
		atoi(match.str(3).c_str()) - 1,
		0,
		atoi(match.str(4).c_str()) - 1
	);
	fTracker->fBuffer[fTracker->fIndex++] = index_t(
		atoi(match.str(5).c_str()) - 1,
		0,
		atoi(match.str(6).c_str()) - 1
	);
	return true;
}

bool processObjectLine(
	std::string line, 
	Object* object, 
	MeshMemoryManager* memManager, 
	Mesh** meshList
) {
	if(object->vTracker->vIndex) {
		if(!loadMeshFromObject(object, memManager, meshList)) {
			return false;
		}
	}
	if(line.substr(2).size() - 2 > MAX_MESH_NAME) {
		return false;
	}
	object->name = line.substr(2);
	STD_LOG("processed object line: " << line);
	return true;
}

bool loadMeshFromObject(
	Object* object, 
	MeshMemoryManager* memManager, 
	Mesh** meshList
) 
{
	Mesh* tMesh = memManager->newMesh(
		object->vTracker->vIndex,
		object->nTracker->nIndex,
		object->fTracker->fIndex,
		object->vTracker->vBuffer,
		object->nTracker->nBuffer,
		object->fTracker->fBuffer
	);
	assert(tMesh);
	if(!tMesh) return false;
	meshList[memManager->meshCount - 1] = tMesh;
	object->reset();
	return true;
}

void Object::reset() {
	name = "";
	vTracker->reset();
	fTracker->reset();
	nTracker->reset();
}

//don't think there's a need to zero the buffer
void VertexTracker::reset() {
	vIndex = 0;
}

//don't think there's a need to zero the buffer
void FaceTracker::reset() {
	fIndex = 0;
}

//don't think there's a need to zero the buffer
void NormalTracker::reset() {
	nIndex = 0;
}

void processNoneLine(std::string line) {
	STD_LOG("processed none line: " << line);
}

bool processVertexLine(std::string line, VertexTracker* vTracker) {
	if(vTracker->vIndex == MAX_MESH_BUFFER_SZ) {
		return false;
	}
	if(!std::regex_match(line.c_str(), match, VERT_PATT)) {
		return false;
	}
	vTracker->vBuffer[vTracker->vIndex++] = vertex_t(
		atof(match.str(1).c_str()),
		atof(match.str(2).c_str()),
		atof(match.str(3).c_str())
	);
	STD_LOG("processed vertex line: " << line);
	return true;
}

#if 0
bool writeBinary(ParsedArgs* pArgs, Mesh** meshList, meshint_t numMeshes) {
	FILE* oFile = fopen(pArgs->outFile.c_str(), "wb+");
	if(oFile == nullptr) {
		STD_ERR("Could not open outFile for writing: " << pArgs->outFile);
		return false;
	}
	MeshFileHeader mFileHeader = MeshFileHeader{
		numMeshes: 1,	
	};
	MeshHeader mHeader = MeshHeader{
		numVerts: object->vTracker->vIndex,
		numIndeces: object->fTracker->fIndex,
		offset: 0,
	};
	//TODO this will need to be expanded to multiple meshes
	//at some point
	//file header
	strcpy(mHeader.name, object->name.c_str());
	size_t numWrite;
	numWrite = fwrite(&mFileHeader, sizeof(MeshFileHeader), 1, oFile);
	assert(numWrite == 1);
	numWrite = fwrite(&mHeader, sizeof(MeshHeader), 1, oFile);
	assert(numWrite == 1);
	numWrite = fwrite(
		object->vTracker->vBuffer, 
		sizeof(vertex_t), 
		object->vTracker->vIndex, oFile
	);
	assert(numWrite == object->vTracker->vIndex);
	numWrite = fwrite(
		object->fTracker->fBuffer, 
		sizeof(vertexindex_t), 
		object->fTracker->fIndex, oFile
	);
	assert(numWrite == object->fTracker->fIndex);
	fclose(oFile);
	return true;
}
#endif
//here
bool writeBinary(ParsedArgs* pArgs, Mesh** meshList, meshint_t numMeshes) {
	return true;
}
