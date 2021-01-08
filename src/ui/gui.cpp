#include "ui/gui.hpp"
#include "platform.hpp"

using namespace UI;

//TODO, what is the ref count of window after this?
//TODO, resizing
//I do realize im hardcoding vulkan here...
GUI::GUI(
	std::shared_ptr<platform::Window> window, 
	std::shared_ptr<graphics::Renderer<graphics::Vulkan>> renderer
) 
: m_window(window)
, m_renderer(renderer)
{
	ImGui::CreateContext();
	m_io = ImGui::GetIO();
	setDisplaySize();

	byte* pixelP = NULL;
	int height, width;
	m_io.Fonts->GetTexDataAsRGBA32(&pixelP, &width, &height);
	std::vector<byte> pixels(pixelP, pixelP + (height * width * 4));
	m_renderer->loadGUITexturesRGBA32(pixels);
    m_io.Fonts->ClearTexData();
}

void GUI::setDisplaySize() {
	int width, height;
	platform::getWindowSize(m_window.get(), height, width);
	m_io.DisplaySize.x = static_cast<float>(width);
	m_io.DisplaySize.y = static_cast<float>(height);
}
