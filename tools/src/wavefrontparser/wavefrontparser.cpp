#include "artemis_asset.hpp"
#include "wavefrontparser.hpp"

#include <stdio.h>

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

bool processFaceLine(std::string line, FaceTracker* fTracker) {
	STD_LOG("processed face line: " << line);
	if(!std::regex_match(line.c_str(), match, FACE_PATT)) {
		return false;
	}
	if(fTracker->fIndex + 3 > MAX_VERTS) {
		return false;
	}
	//should always be multiples of 3, what happens if we end up doing different
	//topologies?
	for(int i = 1; i <= 3; i++) {
		fTracker->fBuffer[fTracker->fIndex++] = static_cast<VertexIndex>((
			atoi(match.str(i).c_str()) - 1)
		);
	}
	return true;
}

bool processObjectLine(std::string line, Object* object) {
	if(line.substr(2).size() - 2 > MAX_ASSET_NAME) {
		return false;
	}
	object->name = line.substr(2);
	STD_LOG("processed object line: " << line);
	return true;
}

void processNoneLine(std::string line) {
	STD_LOG("processed none line: " << line);
}

bool processVertexLine(std::string line, VertexTracker* vTracker) {
	STD_LOG("processed vertex line: " << line);
	if(vTracker->vIndex == MAX_VERTS) {
		return false;
	}
	if(!std::regex_match(line.c_str(), match, VERT_PATT)) {
		return false;
	}
	vTracker->vBuffer[vTracker->vIndex++] = glm::vec3(
		atof(match.str(1).c_str()),
		atof(match.str(2).c_str()),
		atof(match.str(3).c_str())
	);
	return true;
}

bool writeObject(ParsedArgs* pArgs, Object* object) {
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
	strcpy(mHeader.name, object->name.c_str());
	fwrite(&mFileHeader, sizeof(MeshFileHeader), 1, oFile);
	fwrite(&mHeader, sizeof(MeshHeader), 1, oFile);
	fwrite(
		object->vTracker->vBuffer, 
		sizeof(glm::vec3), 
		object->vTracker->vIndex, oFile
	);
	fwrite(
		object->fTracker->fBuffer, 
		sizeof(VertexIndex), 
		object->fTracker->fIndex, oFile
	);
	fclose(oFile);
	return true;
}
