/*
NOTE!!!!! I'm not typedefing the indeces or faces, I feel like memory issues and what not
are going to bite me sooner or later
*/
#include <string>

#include "artemis_game.hpp"
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
	auto mesh = std::unique_ptr<Mesh>(new Mesh);
	auto positionBuffer = std::vector<vertex_pos_t>();
	auto normalBuffer = std::vector<vertex_normal_t>();
	auto textureCoords = std::vector<vertex_tex_coord_t>();
	std::vector<glm::uvec3> faces = std::vector<glm::uvec3>();
	for(;std::getline(inFile, line);) {
		if(line.empty()) continue;
		TYPE type = getTypeFromString(line);
		switch(type) {
			case VERTEX:
			{
				if(!processVertexLine(line, positionBuffer)) {
					QUIT("Failed parsing vertex line:\n\t" << line);
				}
				break;
			}
			case FACE:
			{
				if(!processFaceLine(line, faces)) {
					QUIT("Failed parsing face line:\n\t" << line);
				}
				break;
			}
			case OBJECT:
			{
				if(!processObjectLine(line, mesh.get())) {
					QUIT("Failed parsing object line:\n\t" << line);
				}
				break;
			}
			case NORMAL:
			{
				if(!processNormalLine(line, normalBuffer)) {
					QUIT("Failed parsing normal line:\n\t" << line);
				}
				break;
			}
			case TEXTURE:
			{
				if(!processTextureLine(line, textureCoords)) {
					QUIT("Failed parsing texture line:\n\t" << line);
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
	{
		bool result = makeMesh(positionBuffer, faces, normalBuffer, textureCoords, mesh);
		if(!result) {
			QUIT("failed parsing wavefront object file: '" << pArgs.inFile << "' :(");
		}
		writeBinary(pArgs, *mesh.get());
	};
}
