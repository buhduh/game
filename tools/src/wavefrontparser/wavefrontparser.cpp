#include "artemis_asset.hpp"
#include "wavefrontparser.hpp"

#include <cstdio>

ParsedArgs parseArgs(int argc, char* argv[]) {
	return ParsedArgs{
		inFile: std::string(argv[1]),
		meshName: std::string(argv[2]),
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
	if(nTracker->nIndex == MESHINT_MAX) {
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
	if(!std::regex_match(line.c_str(), match, FACE_PATT)) {
		return false;
	}
	if(fTracker->fIndex + 3 > MESHINT_MAX) {
		return false;
	}

	auto tx1 = match.str(1);
	auto tz1 = match.str(2);
	auto x1 = atoi(tx1.c_str());
	auto z1 = atoi(tz1.c_str());
	auto tx2 = match.str(3);
	auto tz2 = match.str(4);
	auto x2 = atoi(tx2.c_str());
	auto z2= atoi(tz2.c_str());
	auto tx3 = match.str(5);
	auto tz3 = match.str(6);
	auto x3 = atoi(tx3.c_str());
	auto z3= atoi(tz3.c_str());
	//should always be multiples of 3, what happens if we end up doing different
	//topologies?
	fTracker->fBuffer[fTracker->fIndex++] = index_t(x1 - 1, 0, z1 - 1);
	fTracker->fBuffer[fTracker->fIndex++] = index_t(x2 - 1, 0, z2 - 1);
	fTracker->fBuffer[fTracker->fIndex++] = index_t(x3 - 1, 0, z3 - 1);
	STD_LOG("processed face line: " << line);
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

void Object::reset() { name = "";
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
	if(vTracker->vIndex == MESHINT_MAX) {
		return false;
	}
	if(!std::regex_match(line.c_str(), match, VERT_PATT)) {
		return false;
	}
	auto tx = match.str(1);
	auto ty = match.str(2);
	auto tz = match.str(3);
	auto x = atof(tx.c_str());
	auto y = atof(ty.c_str());
	auto z = atof(tz.c_str());
	vTracker->vBuffer[vTracker->vIndex++] = vertex_t(x, y, z);
	STD_LOG("processed vertex line: " << line);
	return true;
}

bool writeBinary(ParsedArgs* pArgs, Mesh** meshList, meshint_t numMeshes) {
	//I know...
	//I changed my mind, will only process one mesh at a time for now
	assert(numMeshes == 1);
	return writeMeshToFile(meshList[0], pArgs->meshName);
}
