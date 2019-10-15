#ifndef WAVEFRONTPARSER_HPP
#define WAVEFRONTPARSER_HPP

#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <regex>

#include "artemis_entity.hpp"

#define STD_LOG(MSG) std::cout << MSG << std::endl
#define STD_ERR(MSG) std::cerr << MSG << std::endl
#define QUIT(MSG) \
	STD_ERR(MSG); \
	exit(EXIT_FAILURE)

enum TYPE {
	VERTEX,
	FACE,
	OBJECT,
	NONE
};

const static std::unordered_map<std::string, TYPE> TYPE_MAP {
	{"v ", VERTEX},
	{"f ", FACE},
	{"o ", OBJECT}
};

struct ParsedArgs {
	std::string inFile;
	std::string outFile;
};

struct VertexTracker {
	VertexIndex vIndex;	
	glm::vec3* vBuffer;
};

struct FaceTracker {
	VertexIndex fIndex;
	VertexIndex* fBuffer;
};

struct Object {
	std::string name;
	VertexTracker* vTracker;
	FaceTracker* fTracker;
};

//definitely not thread safe
static std::cmatch match;
const static std::regex VERT_PATT(
	R"(v (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+)$)");
const static std::regex FACE_PATT(
	R"(f (\d+)/\d*/\d* (\d+)/\d*/\d* (\d+)/\d*/\d*$)");

ParsedArgs parseArgs(int, char**);
bool checkInFile(std::string);
void errorAndBail(std::string);
TYPE getTypeFromString(std::string);
bool processVertexLine(std::string, VertexTracker*);
bool processFaceLine(std::string, FaceTracker*);
void processNoneLine(std::string);
bool processObjectLine(std::string, Object*);
bool writeObject(ParsedArgs*, Object*);

#endif
