/*
* From now on, ALL render interactions pass through this interface.
* TODO, refactor main.cpp and other files so that the above is true, eg,
* 	no more Vulkan::* stuff
*/
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstddef>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "artemis_game.hpp"
#include "platform.hpp"

namespace graphics {
	struct UniformBufferObject {
		alignas(16) mat4 model;
		alignas(16) mat4 view;
		alignas(16) mat4 proj;
	};

	//not even sure about this...
	typedef class Vulkan Renderer;

	//for ease, this matches ImGui::ImDrawVert
	//if i were smart i would get imgui to use the same vertex type as me
	struct GUIVertex {
		vec2     pos;
		vec2     uv;
		uint32_t color; //R8G8B8A8
	};

	//Right now, only the gui system is using these.
	//should these be consistent with meshes?
	typedef uint16_t vertex_index_t;

	/*
	I'm "enamored?" (I guess...?), with using a template here for possibly(probably never)
	porting this to another renderer backend
	I could also do an abstract base class sort of implementation
	template<class T>
	class Renderer {
		public:
		Renderer(platform::Window* window) : backend(std::unique_ptr<T>(new T(window))) {};
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		void loadGUITexturesRGBA32(
				const std::vector<byte>& texture,
				const int& height,
				const int& width
		) {
			backend->loadGUITextureRGBA32(texture, height, width);
		};
		void newFrame() {backend->newFrame();};
		void drawGUI(
			const std::vector<struct UI::Vertex>& vBuffer,
			//not a big fan of this...
			const std::vector<uint16_t>& iBuffer,
			const unsigned int vOffset, 
			const unsigned int iOffset
		) {
			backend->drawGUI(vBuffer, iBuffer, vOffset, iOffset);
		};
		private:
		std::unique_ptr<T> backend;
	};

	std::shared_ptr<Renderer<class Vulkan>> makeDefaultRenderer(platform::Window*);
	*/
};

#include "renderer/vulkan.hpp"

#endif
