#include "artemis_game.hpp"
#include "artemis_asset.hpp"

int main() {
	GameMemory gm = GameMemory(GIGABYTES(1));
	MeshMemoryManager mManager = MeshMemoryManager(&gm);
	Mesh* mesh = loadMeshFromFile(&mManager, std::string("cube"));
	STD_LOG(*mesh);
}
