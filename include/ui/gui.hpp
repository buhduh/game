#ifndef UI_GUI_HPP
#define UI_GUI_HPP

#include <memory>

#include "artemis_game.hpp"
#include "platform.hpp"
#include "imgui/imgui.h"
#include "renderer.hpp"

namespace UI {

	struct Vertex {
		vec2       pos;
		vec2       uv;
		uint32_t   col; //R8G8B8A8
	};

	class GUI {
		public:
			explicit GUI(
				std::shared_ptr<platform::Window>, 
				std::shared_ptr<graphics::Renderer<graphics::Vulkan>>
			);
			GUI(const GUI&) = delete;
			GUI(GUI&&) = delete;
			GUI& operator=(const GUI&) = delete;
		private:
			std::shared_ptr<platform::Window> m_window;
			std::shared_ptr<graphics::Renderer<graphics::Vulkan>> m_renderer;
			ImGuiIO m_io;
			void setDisplaySize();
	};
};
#endif
