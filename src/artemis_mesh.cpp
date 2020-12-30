#include <sstream>

#include "artemis_game.hpp"
#include "artemis_mesh.hpp"

Mesh::Mesh()
	: m_vertexBuffer{vertex_buffer_t{0}}
	, m_faceBuffer{index_buffer_t{0}} 
{}

Mesh::Mesh(const vertex_buffer_t& vertBuffer, const index_buffer_t& indexBuffer) {
	m_vertexBuffer = vertBuffer;
	m_faceBuffer = indexBuffer;
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	this->m_vertexBuffer = mesh.m_vertexBuffer;
	this->m_faceBuffer = mesh.m_faceBuffer;
	return *this;
}

//TODO, pretty sure im not writing vector.data()
//This will change when my asset system gets fancier
//Mesh::operator<< is being used for debuggin, rolling fwrite style here
bool Mesh::writeToAssetFile(const std::string& name) const {
	std::stringstream fName;
	fName << "assets/meshes/" << name << ".bin";
	FILE *oFile = std::fopen(fName.str().c_str(), "wb+");
	assert(oFile);

	std::stringstream failMsg;
	failMsg << "failed opening '" << fName.str() << "' for writing";

	if(!oFile) {
		STD_ERR(failMsg.str());
		return false;
	}

	//vertex buffer length
	auto tLen = m_vertexBuffer.size();
	auto numWrite = std::fwrite(
		&tLen, 
		sizeof(decltype(tLen)), 
		1, oFile
	);
	assert(numWrite == 1);
	if(numWrite != 1) {
		std::fclose(oFile);
		STD_ERR(failMsg.str() << " could not write vertex buffer length");
		return false;
	}

	//vertex buffer
	numWrite = std::fwrite(
		m_vertexBuffer.data(),
		sizeof(Vertex),
		tLen, oFile
	);
	assert(numWrite == tLen);
	if(numWrite != tLen) {
		std::fclose(oFile);
		STD_ERR(failMsg.str() << " could not write vertex buffer");
		return false;
	}

	//index buffer length
	tLen = m_faceBuffer.size();
	numWrite = std::fwrite(
		&tLen,
		sizeof(decltype(tLen)),
		1, oFile
	);
	assert(numWrite == 1);
	if(numWrite != 1) {
		std::fclose(oFile);
		STD_ERR(failMsg.str() << " could not write index buffer length");
		return false;
	}

	//index buffer
	numWrite = std::fwrite(
		m_faceBuffer.data(),
		sizeof(vertex_index_t),
		tLen, oFile
	);
	assert(numWrite == tLen);
	if(numWrite != tLen) {
		std::fclose(oFile);
		STD_ERR(failMsg.str() << " could not write index buffer");
		return false;
	}
	std::fclose(oFile);
	return true;
}

bool Mesh::getFromAssetFile(const std::string& name) {

	std::stringstream fName;
	fName << "assets/meshes/" << name << ".bin";
	std::stringstream failMsg;
	failMsg << "failed opening '" << fName.str() << "' for reading";
	FILE *iFile = std::fopen(fName.str().c_str(), "rb");
	assert(iFile);

	std::size_t numRead;
	decltype(m_vertexBuffer.size()) vertBufferLen = 0;
	decltype(m_faceBuffer.size()) faceBufferLen = 0;

	if(!iFile) {
		STD_ERR(failMsg.str());
		goto fail;
	}

	//m_vertexBuffer len
	numRead = std::fread(
		&vertBufferLen,
		sizeof(decltype(vertBufferLen)),
		1, iFile
	);
	assert(numRead == 1);
	if(numRead != 1) {
		STD_ERR(failMsg.str() << " could not get vertex buffer length");	
		std::fclose(iFile);
		goto fail;
	}
	assert(vertBufferLen);
	if(!vertBufferLen) {
		STD_ERR(failMsg.str() << " could not load vertex buffer length");
		std::fclose(iFile);
		goto fail;
	}

	//m_vertexBuffer
	m_vertexBuffer.resize(vertBufferLen);
	numRead = fread(
		m_vertexBuffer.data(),	
		sizeof(Vertex),
		vertBufferLen, iFile
	);
	assert(numRead == vertBufferLen);
	if(numRead != vertBufferLen) {
		STD_ERR(failMsg.str() << " could not load vertex buffer");
		std::fclose(iFile);
		goto fail;
	}

	//m_faceBuffer len
	numRead = std::fread(
		&faceBufferLen, 
		sizeof(decltype(faceBufferLen)), 
		1, iFile
	);
	assert(numRead == 1);
	if(numRead != 1) {
		STD_ERR(failMsg.str() << " could not get face buffer length");	
		std::fclose(iFile);
		goto fail;
	}
	assert(faceBufferLen);
	if(!faceBufferLen) {
		STD_ERR(failMsg.str() << " could not load face buffer length");
		std::fclose(iFile);
		goto fail;
	}
	m_faceBuffer.resize(faceBufferLen);
	numRead = std::fread(
		m_faceBuffer.data(),
		sizeof(vertex_index_t),
		faceBufferLen, iFile
	);
	assert(numRead == faceBufferLen);
	if(numRead != faceBufferLen) {
		STD_ERR(failMsg.str() << " could not load face buffer");
		std::fclose(iFile);
		goto fail;
	}
	return true;

	fail:
	m_faceBuffer.resize(0);	
	m_vertexBuffer.resize(0);
	*this = Mesh();
	return false;
}

Vertex::Vertex(
		vertex_pos_t p_pos, 
		vertex_color_t p_color, 
		vertex_normal_t p_normal, 
		vertex_tex_coord_t p_textureCoord
)
: pos{p_pos}
, color{p_color}
, normal{p_normal}
, textureCoord{p_textureCoord} {}

/*
TODO
void Mesh::setTexture(const texture_t& texture) {
	m_texels = texture;
}

const char *getTexels() {
	return m_texels.data();
}
*/

//I use this for debug printing, sticking with the c style fwrite for
//moving actual bits around
std::ostream& operator<<(std::ostream& out, const Vertex& vert) {
	std::stringstream po;
	po << "(" << vert.pos.x << "," << vert.pos.y << "," << vert.pos.z << ")";
	std::stringstream co;
	co << "(" << vert.color.x << "," << vert.color.y << "," << vert.color.z << ")";
	std::stringstream no;
	no << "(" << vert.normal.x << "," << vert.normal.y << "," << vert.normal.z << ")";
	std::stringstream to;
	to << "(" << vert.textureCoord.x << "," << vert.textureCoord.y << ")";
	out << "(" << po.str() << ", " << co.str() << ", " << no.str() << ", " << to.str() << ")";
	return out;
}

std::ostream& operator<<(std::ostream &out, const Mesh &mesh) {
	out << "vertex buffer size: " << mesh.m_vertexBuffer.size() << std::endl;
	out << "face buffer size: " << mesh.m_faceBuffer.size() << std::endl;
	for(auto v = mesh.m_vertexBuffer.begin(); v < mesh.m_vertexBuffer.end(); v++) {
		out << "(" << v->pos.x << ", " << v->pos.y << ", " << v->pos.z << ")" << std::endl;
	}
	for(auto f = mesh.m_faceBuffer.begin(); f < mesh.m_faceBuffer.end();) {
		out << "(" << *(f++) << ", " << *(f++) << ", " << *(f++) << ")" << std::endl;
	}
	return out;
}
