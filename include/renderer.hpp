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
		private:
		std::unique_ptr<T> backend;
	};

	std::unique_ptr<Renderer<class Vulkan>> makeDefaultRenderer(platform::Window*);
};

#include "renderer/vulkan.hpp"

#endif
