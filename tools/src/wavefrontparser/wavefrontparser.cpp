//TODO test the buffer realloc functionality
/*
	assumes triangular topology indexed counterclockwise
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <unordered_map>
#include <regex>

#include "wavefrontparser/cli.hpp"
#include "component/model/model.hpp"

const static int CHUNK_SIZE = 1024;

enum TYPE {
	VERTEX,
	FACE
};

const static std::unordered_map<std::string, TYPE> TYPE_MAP {
	{"v ", VERTEX},
	{"f ", FACE}
};

//TODO need to match vertex sizes to acceptable gpu memory inputs
int main(int argc, char* argv[]) {
	Args args = parseCLI(argc, argv);
	std::ifstream inFile(args.input);
	if(inFile.fail()) {
		std::cerr << "Unable to open '" << args.input << "' for reading, stopping." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string line;
	component::model::index_size_t vertCount = 0;
	uint32_t indexCount = 0;
	uint32_t lineN = 1;
	std::regex vertPat(R"(v (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+)$)");
	std::regex facePat(R"(f (\d+)/\d*/\d* (\d+)/\d*/\d* (\d+)/\d*/\d*$)");
	std::cmatch match;
	size_t vertBufferSize = CHUNK_SIZE;
	size_t indexBufferSize = CHUNK_SIZE;
	component::model::vertex* vertBuffer = 
		(component::model::vertex*) 
		malloc(sizeof(component::model::vertex)*vertBufferSize);
	component::model::index_size_t* indexBuffer = 
		(component::model::index_size_t*) 
		malloc(sizeof(component::model::index_size_t)*indexBufferSize);
	for(; std::getline(inFile, line); lineN++) {
		if(line.empty()) continue;
		TYPE type;
		try {
			type = TYPE_MAP.at(line.substr(0, 2));
		} catch(std::out_of_range e) {
			continue;
		}
		switch(type) {
			case VERTEX:
			{
				if(!std::regex_match(line.c_str(), match, vertPat)) {
					std::cerr << "Could not parse vertex line in '" 
						<< args.input << "' line number: "
						<< lineN << std::endl;
					exit(EXIT_FAILURE);
				}
				//*= 2 could really blow this thing up for large v numbers.
				if(vertCount == vertBufferSize) {
					vertBufferSize *= 2;		
					vertBuffer = 
						(component::model::vertex*) 
						realloc(vertBuffer, sizeof(component::model::vertex)*vertBufferSize);
				}
				vertBuffer[vertCount++] = component::model::vertex{
					atof(match.str(1).c_str()),
					atof(match.str(2).c_str()),
					atof(match.str(3).c_str())
				};
				//index reference type can no longer reference vertices
				if(vertCount >=  component::model::INDEX_SIZE_MAX) {
					std::cerr << "Too many vertices for index type to handle!" << std::endl;
					exit(EXIT_FAILURE);
				}
				break;
			}
			case FACE:
			{
				if(!std::regex_match(line.c_str(), match, facePat)) {
					std::cerr << "Could not parse face line in '" 
						<< args.input << "' line number: " 
						<< lineN << std::endl;
					exit(EXIT_FAILURE);
				}
				//should always be multiples of 3, what happens if we end doing different
				//topologies?
				if(indexCount + 3 > indexBufferSize) {
					indexBufferSize *= 2;
					indexBuffer = 
						(component::model::index_size_t*) 
						realloc(indexBuffer, sizeof(component::model::index_size_t)*indexBufferSize);
				}
				indexBuffer[indexCount++] = static_cast<component::model::index_size_t>((
					atoi(match.str(1).c_str()) - 1)
				);
				indexBuffer[indexCount++] = static_cast<component::model::index_size_t>((
					atoi(match.str(2).c_str()) - 1)
				);
				indexBuffer[indexCount++] = static_cast<component::model::index_size_t>((
					atoi(match.str(3).c_str()) - 1)
				);
				break;
			}
		}
	}
	inFile.close();
	FILE* oFile = fopen(args.output.c_str(), "wb+");
	if(oFile == nullptr) {
		//I should probably try to open this at start of program just to save time for problems
		std::cerr << "Failed opening '" << args.output << "' for writing, stopping." << std::endl;
		exit(EXIT_FAILURE);
	}
	component::model::ModelHeader mHeader = {vertCount, indexCount};
	fwrite(&mHeader, sizeof(component::model::ModelHeader), 1, oFile);
	fwrite(vertBuffer, sizeof(component::model::vertex), vertCount, oFile);
	fwrite(indexBuffer, sizeof(component::model::index_size_t), indexCount, oFile);
	fclose(oFile);
	free(vertBuffer);
	free(indexBuffer);
}
