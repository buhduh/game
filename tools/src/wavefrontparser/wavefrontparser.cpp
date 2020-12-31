#include <cstdio>

#include <sstream>

#include "wavefrontparser.hpp"

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

bool processNormalLine(std::string line, std::vector<vertex_normal_t>& normals) {
	if(!std::regex_match(line.c_str(), match, NORMAL_PATT)) {
		return false;
	}
	normals.push_back(
		vertex_normal_t(
			atof(match.str(1).c_str()),
			atof(match.str(2).c_str()),
			atof(match.str(3).c_str())
		)
	);
	//STD_LOG("processed normal line: " << line);
	return true;
}

bool processFaceLine(std::string line, std::vector<glm::uvec3>& faces) {
	if(!std::regex_match(line.c_str(), match, FACE_PATT)) {
		return false;
	}

	//first triplet
	auto tx1 = match.str(1);
	auto ty1 = match.str(2);
	auto tz1 = match.str(3);
	auto x1 = atoi(tx1.c_str());
	auto tty1 = atoi(ty1.c_str());
	auto y1 = tty1 ? tty1 : 1;
	auto z1 = atoi(tz1.c_str());

	//second triplet
	auto tx2 = match.str(4);
	auto ty2 = match.str(5);
	auto tz2 = match.str(6);
	auto x2 = atoi(tx2.c_str());
	auto tty2 = atoi(ty2.c_str());
	auto y2 = tty2 ? tty2 : 1;
	auto z2= atoi(tz2.c_str());

	//third triplet
	auto tx3 = match.str(7);
	auto ty3 = match.str(8);
	auto tz3 = match.str(9);
	auto x3 = atoi(tx3.c_str());
	auto tty3 = atoi(ty3.c_str());
	auto y3 = tty3 ? tty3 : 1;
	auto z3= atoi(tz3.c_str());

	//should always be multiples of 3, what happens if we end up doing different
	//topologies? it crashes when not triangulated in blender
	faces.push_back(glm::uvec3(x1 - 1, y1 - 1, z1 - 1));
	faces.push_back(glm::uvec3(x2 - 1, y2 - 1, z2 - 1));
	faces.push_back(glm::uvec3(x3 - 1, y3 - 1, z3 - 1));
	//STD_LOG("processed face line: " << line);
	return true;
}

bool processObjectLine(std::string line, Mesh *mesh) {
	/*
	//Not quite sure if i even need this...
	TODO
	if(object->vTracker->vIndex) {
		if(!loadMeshFromObject(object, memManager, meshList)) {
			return false;
		}
	}
	if(line.substr(2).size() - 2 > MAX_MESH_NAME) {
		return false;
	}
	object->name = line.substr(2);
	//STD_LOG("processed object line: " << line);
	*/
	return true;
}

void processNoneLine(std::string line) {
	//STD_LOG("processed none line: " << line);
}

bool processTextureLine(std::string line, std::vector<vertex_tex_coord_t>& texture) {
	if(!std::regex_match(line.c_str(), match, TEXTURE_PATT)) {
		return false;
	}
	auto tu = match.str(1);
	auto tv = match.str(2);
	auto u = atof(tu.c_str());
	auto v = atof(tv.c_str());
	if(u > 1.0f || u < 0.0f) {
		STD_ERR("Texture U,V coordinates must be between 0.0 and 1.0");
		return false;
	}
	if(v > 1.0f || v < 0.0f) {
		STD_ERR("Texture U,V coordinates must be between 0.0 and 1.0");
		return false;
	}
	texture.push_back(vertex_tex_coord_t(u,v));
	return true;
}

bool processVertexLine(std::string line, std::vector<vertex_pos_t>& positions) {
	if(!std::regex_match(line.c_str(), match, VERT_PATT)) {
		return false;
	}
	auto tx = match.str(1);
	auto ty = match.str(2);
	auto tz = match.str(3);
	auto x = atof(tx.c_str());
	auto y = atof(ty.c_str());
	auto z = atof(tz.c_str());
	positions.push_back(vertex_pos_t(x,y,z));
	//STD_LOG("processed vertex line: " << line);
	return true;
}

bool writeBinary(const ParsedArgs& pArgs, const Mesh& mesh) {
	return mesh.writeToAssetFile(pArgs.meshName);
}

typedef std::unordered_map<Vertex, long unsigned int> vertex_map_t;
bool makeMesh(
	const std::vector<vertex_pos_t>& positions, 
	const std::vector<glm::uvec3>& indeces, 
	const std::vector<vertex_normal_t>& normals, 
	const std::vector<vertex_tex_coord_t>& textureCoords, 
	std::unique_ptr<Mesh>& mesh
) {
	vertex_map_t vertMap;
	vertex_buffer_t vertexBuffer(0);
	index_buffer_t indexBuffer(indeces.size());
	for(long unsigned int i = 0; i < indeces.size(); i++) {
		auto newPos = vertex_pos_t(positions[indeces[i].x]);		
		auto newColor = glm::vec3(1.0f, 1.0f, 0.0f);
		auto newNormal = vertex_normal_t(normals[indeces[i].z]);
		auto newTextCoord = vertex_tex_coord_t(0.0f, 0.0f);
		if(textureCoords.size()) {
			newTextCoord = vertex_tex_coord_t(textureCoords[indeces[i].y]);
		}
		Vertex newVert = Vertex(newPos, newColor, newNormal, newTextCoord);
		auto res = vertMap.try_emplace(newVert, i);
		if(res.second) {
			vertexBuffer.push_back(newVert);
			//This is i, but this feels more "correct" to me, idk
			indexBuffer[i] = 
				static_cast<vertex_index_t>(vertexBuffer.size() - 1);
		} else {
			indexBuffer[i] = static_cast<vertex_index_t>(res.first->second);
		}
	}
	*mesh.get() = Mesh(vertexBuffer, indexBuffer);
	STD_LOG("index buffer length: " << indexBuffer.size() << ", vertex buffer length: " << vertexBuffer.size());
	return true;
}
