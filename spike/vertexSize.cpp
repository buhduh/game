#include "artemis_game.hpp"
#include "artemis_mesh.hpp"

int main(void) {
	STD_LOG("vertex: " << sizeof(Vertex));
	STD_LOG("pos: " << sizeof(vertex_pos_t));
	STD_LOG("color: " << sizeof(vertex_color_t));
	STD_LOG("normal: " << sizeof(vertex_normal_t));
	STD_LOG("texture: " << sizeof(vertex_tex_coord_t));
	Mesh emptyM = Mesh();
	Mesh sphere;
	sphere.getFromAssetFile("sphere");
	STD_LOG("empty: " << sizeof(emptyM));
	STD_LOG("sphere: " << sizeof(sphere));
	STD_LOG("facebuffer: " << sizeof(index_buffer_t));
	STD_LOG("vertexBuffer: " << sizeof(vertex_buffer_t));
}
