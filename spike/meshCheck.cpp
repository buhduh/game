#include "artemis_game.hpp"
#include "artemis_mesh.hpp"

#include <unordered_map>

//the reindexing wavefrontparser algorithm is busted :(
int main(void) {
	Mesh mesh;
	//mesh.getFromAssetFile("cube");
	mesh.getFromAssetFile("sphere");
	std::unordered_map<Vertex, int> set;
	int i = 0;
	auto vBuffer = mesh.getVertexBuffer();
	for(auto v = vBuffer.begin(); v != vBuffer.end(); v++) {
		auto huh = set.try_emplace(*v, i);
		if(!huh.second) {
			if(vBuffer[huh.first->second] == huh.first->first) {
				STD_LOG("vertex at " << huh.first->second << " vertex: " << vBuffer[huh.first->second]);
				STD_LOG("current vertex i: " << i << " vertex: " << huh.first->first);
			}
		}
		i++;
	}
	STD_LOG("size: " << mesh.getVertexBuffer().size() << " i: " << i);
}
