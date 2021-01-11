#ifndef ARTEMIS_MESH_HPP
#define ARTEMIS_MESH_HPP

#include <vector>
#include <ostream>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "artemis_game.hpp"

typedef vec3 vertex_pos_t;
typedef vec3 vertex_color_t;
typedef vec3 vertex_normal_t;
typedef vec2 vertex_tex_coord_t;
typedef uint16_t  vertex_index_t;

//Probably need to deal with alignments...
//TODO do i need to move this to the render namespace?
struct Vertex{
	//Data
	vertex_pos_t       pos;
	vertex_color_t     color;
	vertex_normal_t    normal;
	vertex_tex_coord_t textureCoord;
	//end data

	Vertex(vertex_pos_t, vertex_color_t, vertex_normal_t, vertex_tex_coord_t);
	Vertex() = default;
	//Probably don't want to delete these but I obviously will be doing
	//memory "stuff" with them at some point
	//Vertex(Vertex&&) = delete;
	//Vertex(const Vertex&) = delete;
	friend std::ostream& operator<<(std::ostream&, const Vertex&);
	inline bool operator==(const Vertex& rhs) const {
        return pos          == rhs.pos    && 
               color        == rhs.color  && 
               normal       == rhs.normal && 
               textureCoord == rhs.textureCoord;
	};
	inline bool operator!=(const Vertex& rhs) const {
		return !(*this == rhs);
	};
};

typedef std::vector<Vertex> vertex_buffer_t;
typedef std::vector<vertex_index_t> index_buffer_t;

class Mesh{
	public:
	Mesh();
	explicit Mesh(const vertex_buffer_t&, const index_buffer_t&);
	//Will probably enable these once i get the ecs working with the "real" memory system
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	Mesh& operator=(const Mesh&);
	//TODO gonna need to figure this one out
	//void setTexture(const texture_t&);
	//const char *getTexels();
	friend std::ostream& operator<<(std::ostream&, const Mesh&);
	bool writeToAssetFile(const std::string&) const;
	bool getFromAssetFile(const std::string&);
	index_buffer_t getIndexBuffer() const;
	vertex_buffer_t getVertexBuffer() const;
	//TODO figure out why the first const has no effect
	//const vertex_buffer_t getVertexBuffer() const;

	private:
	vertex_buffer_t m_vertexBuffer;
	index_buffer_t  m_indexBuffer;
	//TODO gonna need to figure this one out
	//texture_t m_texels;
};

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
