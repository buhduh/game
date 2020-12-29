#include <sstream>

#include "artemis_mesh.hpp"

Mesh::Mesh()
	: m_vertexBuffer{vertex_buffer_t{0}}
	, m_faceBuffer{index_buffer_t{0}} 
{}

Mesh::Mesh(const vertex_buffer_t& vertBuffer, const index_buffer_t& indexBuffer) {
	//copy constructors, this is the ecs "magic"
	m_vertexBuffer = vertBuffer;
	m_faceBuffer = indexBuffer;
}

/*
TODO
void Mesh::setTexture(const texture_t& texture) {
	m_texels = texture;
}

const char *getTexels() {
	return m_texels.data();
}
*/

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
