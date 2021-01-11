#ifndef UI_GUI_HPP
#define UI_GUI_HPP

#include <memory>
#include <vector>

#include "artemis_game.hpp"
#include "platform.hpp"
#include "imgui/imgui.h"
#include "renderer.hpp"

namespace ui {

	class GUI {
		public:
			explicit GUI(
				std::shared_ptr<platform::Window>, 
				std::shared_ptr<graphics::Renderer>
			);
			~GUI();
			GUI(const GUI&) = delete;
			GUI(GUI&&) = delete;
			GUI& operator=(const GUI&) = delete;
			void newFrame();
		private:
			std::shared_ptr<platform::Window> m_window;
			std::shared_ptr<graphics::Renderer> m_renderer;
			void setDisplaySize();
			ImGuiIO* m_io;
	};
};
#endif
