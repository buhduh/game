#ifndef WAVEFRONTPARSER_HPP
#define WAVEFRONTPARSER_HPP

#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <regex>

#include "artemis_game.hpp"
#include "artemis_mesh.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

enum TYPE {
	VERTEX,
	FACE,
	OBJECT,
	NORMAL,
	NONE,
	TEXTURE
};

const static std::unordered_map<std::string, TYPE> TYPE_MAP {
	{"v ", VERTEX},
	{"f ", FACE},
	{"o ", OBJECT},
	{"vn", NORMAL},
	{"vt", TEXTURE},

};

struct ParsedArgs {
	std::string inFile;
	std::string meshName;
};

//definitely not thread safe
static std::cmatch match;
const static std::regex VERT_PATT(
	R"(v (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+)$)");
//TODO, texture is the second one...
const static std::regex FACE_PATT(
	R"(f (\d+)/(\d*)/(\d+) (\d+)/(\d*)/(\d+) (\d+)/(\d*)/(\d+)$)");
const static std::regex NORMAL_PATT(
	R"(vn (-?\d+\.\d+) (-?\d+\.\d+) (-?\d+\.\d+)$)");
const static std::regex TEXTURE_PATT(
	R"(vt ((?:0|1)\.\d+) ((?:0|1)\.\d+)$)");

ParsedArgs parseArgs(int, char**);
bool checkInFile(std::string);
void errorAndBail(std::string);
TYPE getTypeFromString(std::string);
bool processVertexLine(std::string, std::vector<vertex_pos_t>&);
bool processFaceLine(std::string, std::vector<glm::uvec3>&);
bool processNormalLine(std::string, std::vector<vertex_normal_t>&);
bool processTextureLine(std::string, std::vector<vertex_tex_coord_t>&);
void processNoneLine(std::string);
bool processObjectLine(std::string, Mesh*);
bool writeBinary(const ParsedArgs&, const Mesh&);

//std::unique_ptr<Mesh>&: in, the rest out
bool makeMesh(
	const std::vector<vertex_pos_t>&, 
	const std::vector<glm::uvec3>&, 
	const std::vector<vertex_normal_t>&, 
	const std::vector<vertex_tex_coord_t>&, 
	std::unique_ptr<Mesh>&
);

//for unordered_map<Vertex, unsigned long int>
template<>
struct std::hash<Vertex> {
	std::size_t operator()(const Vertex& vert) const {
		auto posHash = std::hash<vertex_pos_t>()(vert.pos);
		auto colorHash = std::hash<vertex_color_t>()(vert.color);
		auto normalHash = std::hash<vertex_normal_t>()(vert.normal);
		auto textHash = std::hash<vertex_tex_coord_t>()(vert.textureCoord);
		return posHash ^ colorHash ^ normalHash ^ textHash;
	}
};
#endif
