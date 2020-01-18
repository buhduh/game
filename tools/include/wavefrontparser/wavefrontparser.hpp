#ifndef WAVEFRONTPARSER_HPP
#define WAVEFRONTPARSER_HPP

#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <regex>

#include "artemis_mesh.hpp"

#define STD_ERR(MSG) std::cerr << MSG << std::endl
#define QUIT(MSG) \
	STD_ERR(MSG); \
	exit(EXIT_FAILURE)

enum TYPE {
	VERTEX,
	FACE,
	OBJECT,
	NORMAL,
	NONE
};

const static std::unordered_map<std::string, TYPE> TYPE_MAP {
	{"v ", VERTEX},
	{"f ", FACE},
	{"o ", OBJECT},
	{"vn", NORMAL}
};

struct ParsedArgs {
	std::string inFile;
	std::string outFile;
};

struct VertexTracker {
	meshint_t vIndex;	
	vertexbuffer_t vBuffer;
	void reset();
};

struct FaceTracker {
	meshint_t fIndex;
	indexbuffer_t fBuffer;
	void reset();
};

struct NormalTracker {
	meshint_t nIndex;
	normalbuffer_t nBuffer;
	void reset();
};

struct Object {
	std::string name;
	VertexTracker* vTracker;
	FaceTracker* fTracker;
	NormalTracker* nTracker;
	void reset();
};

//definitely not thread safe
static std::cmatch match;
const static std::regex VERT_PATT(
	R"(v (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+)$)");
const static std::regex FACE_PATT(
	R"(f (\d+)/\d*/(\d+) (\d+)/\d*/(\d+) (\d+)/\d*/(\d+)$)");
const static std::regex NORMAL_PATT(
	R"(vn (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+)$)");

ParsedArgs parseArgs(int, char**);
bool checkInFile(std::string);
void errorAndBail(std::string);
TYPE getTypeFromString(std::string);
bool processVertexLine(std::string, VertexTracker*);
bool processFaceLine(std::string, FaceTracker*);
bool processNormalLine(std::string, NormalTracker*);
void processNoneLine(std::string);
bool processObjectLine(std::string, Object*, MeshMemoryManager*, Mesh**);
bool writeBinary(ParsedArgs*, Mesh**, meshint_t);
bool loadMeshFromObject(Object*, MeshMemoryManager*, Mesh**);

#endif
