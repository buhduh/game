#ifndef ARTEMIS_ENTITY_HPP
#define ARTEMIS_ENTITY_HPP

#include <glm/glm.hpp>
#include <cstdint>

#define MAX_VERTS UINT16_MAX
typedef uint16_t VertexIndex ;

struct Mesh {
  glm::vec3* vertices;
  VertexIndex* indeces;  
};

struct Entity {
  Mesh *mesh;
};

#endif
