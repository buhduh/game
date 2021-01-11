#include "ui/gui.hpp"
#include "platform.hpp"

using namespace ui;

bool assertTest() {
	if(sizeof(graphics::GUIVertex) != sizeof(ImDrawVert)) {
		return false;
	}
	ImDrawVert imVert = {};
	imVert.pos.x = 1.0f;
	imVert.pos.y = 22.4f;
	imVert.uv.x = 23.9f;
	imVert.uv.y = 23.3f;
	imVert.col = 3454;
	graphics::GUIVertex* myVert = reinterpret_cast<graphics::GUIVertex*>(&imVert);
	assert(myVert->pos.x == imVert.pos.x);
	assert(myVert->pos.y == imVert.pos.y);
	assert(myVert->uv.x == imVert.uv.x);
	assert(myVert->uv.y == imVert.uv.y);
	assert(myVert->col == imVert.col);
	return true;
}

//TODO, resizing
//TODO, does window and renderer ref count increment?
//I do realize im hardcoding vulkan here...
GUI::GUI(
	std::shared_ptr<platform::Window> window, 
	std::shared_ptr<graphics::Renderer> renderer
) 
: m_window(window)
, m_renderer(renderer)
{
	//just a stupid thing to give me some assurance it will do what it should
	assert(assertTest());
	ImGui::CreateContext();
	m_io = &ImGui::GetIO();
	/*
	enables vtx offsets and idx offsets in the 
	returned index and vertex buffers;
	Im not going to worry about this, there's a vector<ImDrawCmd> where
	ImDrawCmd->VtxOffset and ImDrawCmd->IdxOffset, but I don't really get
	how that fits with the vertex/index buffers, i can just do the offsets myself
	if that bit is slow I can investigate using this bit
	m_io->BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	*/
	setDisplaySize();

	byte* pixelP = NULL;
	int height, width;
	m_io->Fonts->GetTexDataAsRGBA32(&pixelP, &width, &height);
	std::vector<byte> pixels(pixelP, pixelP + (height * width * 4));
	m_renderer->loadGUITextureRGBA32(pixels, height, width);
    m_io->Fonts->ClearTexData();
}

GUI::~GUI() {
	ImGui::DestroyContext();
}

void GUI::setDisplaySize() {
	int width, height;
	platform::getWindowSize(m_window.get(), height, width);
	m_io->DisplaySize.x = static_cast<float>(width);
	m_io->DisplaySize.y = static_cast<float>(height);
}

void GUI::newFrame() {
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;
	static bool show_demo_window = false;
	static bool show_another_window = false;
	ImGui::Begin("Hello, world!");
	ImGui::Text("This is some useful text.");
	ImGui::Checkbox("Demo Window", &show_demo_window);
	ImGui::Checkbox("Another Window", &show_another_window);
	//TODO
	//ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Button"))
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	ImGui::Text(
		"Application average %.3f ms/frame (%.1f FPS)", 1000.0f / 
		m_io->Framerate, m_io->Framerate
	);
	ImGui::End();
	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	for(int i = 0; i < drawData->CmdListsCount; i++) {
		const ImDrawList* cmdList = drawData->CmdLists[i];
		const graphics::GUIVertex* vStart = 
			reinterpret_cast<graphics::GUIVertex* const>(cmdList->VtxBuffer.Data);
		std::vector<graphics::GUIVertex> vertBuffer(
			vStart, vStart + cmdList->VtxBuffer.Size
		);
		const graphics::vertex_index_t* iStart = 
			reinterpret_cast<graphics::vertex_index_t* const>(cmdList->IdxBuffer.Data);
		std::vector<graphics::vertex_index_t>indexBuffer(
			iStart, iStart + cmdList->IdxBuffer.Size
		);
		m_renderer->drawGUI(vertBuffer, indexBuffer);
	}
}
