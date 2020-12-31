#include "artemis_game.hpp"
#include "artemis_mesh.hpp"

int main(void) {}

/*
this doesn't work b/c Mesh::m_vertexBuffer is private
make it public if you want to run it
int main(void) {
	Mesh mesh;
	mesh.getFromAssetFile("sphere");
	vertex_pos_t tVec = mesh.m_vertexBuffer[0].pos;
	auto maxX = tVec.x, maxY = tVec.y, maxZ = tVec.z;
	auto minX = maxX, minY = maxY, minZ = maxZ;
	int maxXI = 0, maxYI = 0, maxZI = 0;
	int minXI = 0, minYI = 0, minZI = 0;
	int i = 0;
	for(auto v = mesh.m_vertexBuffer.begin(); v < mesh.m_vertexBuffer.end(); v++,i++) {
		auto tPos = v->pos;
		if(tPos.x > maxX) {
			maxX = tPos.x;
			maxXI = i;
		}
		if(tPos.y > maxY) {
			maxY = tPos.y;
			maxYI = i;
		}
		if(tPos.z > maxZ) {
			maxZ = tPos.z;
			maxZI = i;
		}
		if(tPos.x < minX) {
			minX = tPos.x;
			minXI = i;
		}
		if(tPos.y < minY) {
			minY = tPos.y;
			minYI = i;
		}
		if(tPos.z < minZ) {
			minZ = tPos.z;
			minZI = i;
		}
	}
	auto vBuff = mesh.m_vertexBuffer;
	STD_LOG("max x: " << maxXI << ", " << vBuff[maxXI]);
	STD_LOG("max y: " << maxYI << ", " << vBuff[maxYI]);
	STD_LOG("max z: " << maxZI << ", " << vBuff[maxZI]);
	STD_LOG("min x: " << minXI << ", " << vBuff[minXI]);
	STD_LOG("min y: " << minYI << ", " << vBuff[minYI]);
	STD_LOG("min z: " << minZI << ", " << vBuff[minZI]);
}
*/
