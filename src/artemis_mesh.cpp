#include <sstream>

#include "artemis_game.hpp"
#include "artemis_mesh.hpp"

Mesh::Mesh()
	: m_vertexBuffer{vertex_buffer_t{0}}
	, m_indexBuffer{index_buffer_t{0}} 
{}

Mesh::Mesh(const vertex_buffer_t& vertBuffer, const index_buffer_t& indexBuffer) {
	m_vertexBuffer = vertBuffer;
	m_indexBuffer = indexBuffer;
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	this->m_vertexBuffer = mesh.m_vertexBuffer;
	this->m_indexBuffer = mesh.m_indexBuffer;
	return *this;
}

index_buffer_t Mesh::getIndexBuffer() const {
	return m_indexBuffer;
}

vertex_buffer_t Mesh::getVertexBuffer() const {
	return m_vertexBuffer;
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
	failMsg << "failed writing " << fName.str() << "reason: ";

	if(!oFile) {
		STD_ERR(failMsg.str() << " could not open for writing");
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
	tLen = m_indexBuffer.size();
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
		m_indexBuffer.data(),
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
	failMsg << "failed reading " << fName.str() << " reason: ";
	FILE *iFile = std::fopen(fName.str().c_str(), "rb");
	assert(iFile);

	std::size_t numRead;
	decltype(m_vertexBuffer.size()) vertBufferLen = 0;
	decltype(m_indexBuffer.size()) indexBufferLen = 0;

	if(!iFile) {
		STD_ERR(failMsg.str() << "could not open for reading");
		goto fail;
	}

	//m_vertexBuffer len
	numRead = std::fread(
		&vertBufferLen,
		sizeof(decltype(vertBufferLen)),
		1, iFile
	);
	if(numRead != 1) {
		STD_ERR(failMsg.str() << "could not get vertex buffer length");	
		goto fail;
	}
	if(!vertBufferLen) {
		STD_ERR(failMsg.str() << "could not load vertex buffer length");
		goto fail;
	}

	//m_vertexBuffer
	m_vertexBuffer.resize(vertBufferLen);
	numRead = fread(
		m_vertexBuffer.data(),	
		sizeof(Vertex),
		vertBufferLen, iFile
	);
	if(numRead != vertBufferLen) {
		STD_ERR(failMsg.str() << "could not load vertex buffer");
		goto fail;
	}

	//m_indexBuffer len
	numRead = std::fread(
		&indexBufferLen, 
		sizeof(decltype(indexBufferLen)), 
		1, iFile
	);
	if(numRead != 1) {
		STD_ERR(failMsg.str() << "could not get index buffer length");	
		goto fail;
	}
	if(!indexBufferLen) {
		STD_ERR(failMsg.str() << "could not load index buffer length");
		goto fail;
	}

	//m_indexBuffer
	m_indexBuffer.resize(indexBufferLen);
	numRead = std::fread(
		m_indexBuffer.data(),
		sizeof(vertex_index_t),
		indexBufferLen, iFile
	);
	if(numRead != indexBufferLen) {
		STD_ERR(failMsg.str() << "could not load index buffer");
		goto fail;
	}
	return true;

	fail:
	//I was doing these around the failed if statements,
	//but i think a clean assert(true) works best here
	std::fclose(iFile);
	assert(true);
	m_indexBuffer.resize(0);	
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

//I use this for debug printing, sticking with the c style fwrite for
//moving actual bits around
std::ostream& operator<<(std::ostream &out, const Mesh &mesh) {
	out << "vertex buffer size: " << mesh.m_vertexBuffer.size() << std::endl;
	out << "index buffer size: " << mesh.m_indexBuffer.size() << std::endl;
	for(auto v = mesh.m_vertexBuffer.begin(); v < mesh.m_vertexBuffer.end(); v++) {
		out << *v << std::endl;
	}
	for(auto f = mesh.m_indexBuffer.begin(); f < mesh.m_indexBuffer.end();) {
		out << "(" << *(f++) << ", " << *(f++) << ", " << *(f++) << ")" << std::endl;
	}
	return out;
}
