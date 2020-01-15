#include <string>

#include "wavefrontparser.hpp"

#define EXPECTED_ARGS 3
#define TOOL "wavefrontparser"

//Only parsing one file for now...
int main(int argc, char* argv[]) {
	ParsedArgs pArgs = parseArgs(argc, argv);
	if(argc != EXPECTED_ARGS) {
		QUIT(TOOL << " expects arguments" << EXPECTED_ARGS << ", " << argc << " given.");
	}
	if(!checkInFile(pArgs.inFile)) {
		QUIT("Unable to find and/or open inFile: " << pArgs.inFile);
	}
	std::ifstream inFile(pArgs.inFile, std::ifstream::in|std::ifstream::binary);
	std::string line;
	vertex_t vertBuffer[MAX_MESH_BUFFER_SZ] = {vertex_t(0)};
	index_t indexBuffer[MAX_MESH_BUFFER_SZ] = {index_t(0)};
	normal_t normalBuffer[MAX_MESH_BUFFER_SZ] = {normal_t(0)};
	VertexTracker vTracker = VertexTracker{
		vIndex: 0,
		vBuffer: vertBuffer,
	};
	FaceTracker fTracker = FaceTracker{
		fIndex: 0,
		fBuffer: indexBuffer,
	};
	NormalTracker nTracker = NormalTracker{
		nIndex: 0,
		nBuffer: normalBuffer,
	};
	Object object = Object{
		name: std::string(""),
		vTracker: &vTracker,
		fTracker: &fTracker,
		nTracker: &nTracker,
	};
	for(;std::getline(inFile, line);) {
		if(line.empty()) continue;
		TYPE type = getTypeFromString(line);
		switch(type) {
			case VERTEX:
			{
				if(!processVertexLine(line, &vTracker)) {
					QUIT("Failed parsing vertex line:\n\t" << line);
				}
				break;
			}
			case FACE:
			{
				if(!processFaceLine(line, &fTracker)) {
					QUIT("Failed parsing face line:\n\t" << line);
				}
				break;
			}
			case OBJECT:
			{
				if(!processObjectLine(line, &object)) {
					QUIT("Failed parsing object line:\n\t" << line);
				}
				break;
			}
			case NORMAL:
			{
				if(!processNormalLine(line, &nTracker)) {
					QUIT("Failed parsing normal line:\n\t" << line);
				}
				break;
			}
			case NONE:
			{
				processNoneLine(line);
				break;
			}
		}
	}
	writeObject(&pArgs, &object);
}
