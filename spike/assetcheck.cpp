#include "artemis_asset.hpp"
#include "artemis_memory.hpp"
#include "artemis_mesh.hpp"

#include <string>

int main(void) {
	IArena* arena = new StupidArena();
	MeshOrganizer mOrganizer(arena);
	Mesh mesh1 = getMeshAsset("Cube", &mOrganizer);
	Mesh mesh2 = getMeshAsset("Cube", &mOrganizer);
	STD_LOG(mesh1);
	STD_LOG(mesh2);
}
