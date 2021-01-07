#include "artemis_game.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"

int main(void) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	int width, height;
	unsigned char* pixels = NULL;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	STD_LOG("height: " << height << " width: " << width );

	FILE *oFile = std::fopen("fonts", "wb+");

	auto numWrite = std::fwrite(
		pixels, 
		sizeof(unsigned char), 
		height * width * 4, oFile
	);
}
