#include "artemis_asset.hpp"
#include "artemis_memory.hpp"
#include "artemis_mesh.hpp"

#include <string>

int main(void) {
	IArena* arena = new StupidArena();
	MeshOrganizer mOrganizer(arena);
	Mesh mesh = getMeshAsset("Cube", &mOrganizer);
}
