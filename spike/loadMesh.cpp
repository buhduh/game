#include "artemis_mesh.hpp"
#include "artemis_game.hpp"

int main(void) {
	Mesh mesh;
	mesh.getFromAssetFile("sphere");
	STD_LOG(mesh);
}
