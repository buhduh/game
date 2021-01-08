#ifndef UI_UI_HPP
#define UI_UI_HPP

/*
namespace UI {


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	initImGUI(window, io);

	doImGUIFrame();
	ImDrawData* draw_data = ImGui::GetDrawData();

void doImGUIFrame() {
	static float f = 0.0f;
	static int counter = 0;

	static float clear_color[4] = {
		0.0f, 0.0f, 0.0f, 0.0f	
	};

	static bool show_demo_window;
	static bool show_another_window;

	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
}

//probably need to do something on the heap
void initImGUI(platform::Window* window, ImGuiIO& io) {
	TODO: Set optional io.ConfigFlags values, e.g. 
		'io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard' 
		to enable keyboard controls.
	TODO: Fill optional fields of the io structure later.
	TODO: Load TTF/OTF fonts if you don't want to use the default font.

	int width, height;
	unsigned char* pixels = NULL;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	int wHeight, wWidth;
	platform::getWindowSize(window, wHeight, wWidth);

	STD_LOG("height: " << (float) wHeight << ", width: " << (float) wWidth);

	io.DisplaySize.x = (float) wWidth;             // set the current display width
	io.DisplaySize.y = (float) wHeight;             // set the current display height here
}



};

*/
#endif
