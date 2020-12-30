#ifndef ARTEMIS_MESH_HPP
#define ARTEMIS_MESH_HPP

#include <vector>
#include <ostream>
#include <glm/glm.hpp>

typedef glm::vec3 vertex_pos_t;
typedef glm::vec3 vertex_color_t;
typedef glm::vec3 vertex_normal_t;
typedef glm::vec2 vertex_tex_coord_t;
typedef uint16_t  vertex_index_t;

//Probably need to deal with alignments...
struct Vertex{
	vertex_pos_t       pos;
	vertex_color_t     color;
	vertex_normal_t    normal;
	vertex_tex_coord_t textureCoord;
	Vertex(vertex_pos_t, vertex_color_t, vertex_normal_t, vertex_tex_coord_t);
	Vertex() = default;
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

	private:
	vertex_buffer_t m_vertexBuffer;
	index_buffer_t  m_faceBuffer;
	//TODO gonna need to figure this one out
	//texture_t m_texels;
};

#endif
