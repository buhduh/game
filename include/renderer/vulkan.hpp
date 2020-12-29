#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>

#include <GLFW/glfw3.h>

#include "platform.hpp"

namespace renderer {

class Vulkan  {
	public:
	Vulkan(platform::Window*);
	~Vulkan();
	void drawFrame(UniformBufferObject* ubo);
	void waitIdle();
	private:
	const static std::vector<const char*> deviceExtensions;
	const static size_t MAX_FRAMES_IN_FLIGHT;

	//members
	std::vector<const char*> extensions;
	bool frameBufferResized;
	std::vector<const char*> validationLayers;
	VkInstance instance;
	platform::Window* window;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VkDebugUtilsMessengerEXT debugger;

	//functions
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT,
		VkDebugUtilsMessageTypeFlagsEXT,
		const VkDebugUtilsMessengerCallbackDataEXT*,
		void*
	);
	void cleanupSwapChain();
	void setExtensions();
	void setValidationLayers();
	void createInstance();
	void createSurface();
	void createDebugger();
	void createPhysicalDevice();
	void createSyncObjects();
	bool checkDeviceExtensionSupport(VkPhysicalDevice);
	uint32_t getQueueFamilyIndex(VkPhysicalDevice);
	struct SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice);
	void selectSuitableDevice(const std::vector<VkPhysicalDevice>);
	void createGraphicsPipeline();
	void createLogicalDeviceAndQueue();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createFramebuffers();
	void createVertexBuffer();
	void updateUniformBuffer(uint32_t, UniformBufferObject*);
	void createCommandBuffers();
	std::vector<VkImageView> swapChainImageViews;
	void createCommandPool();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>);
	VkPresentModeKHR chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>);
	VkExtent2D chooseSwapExtent(
		const VkSurfaceCapabilitiesKHR&);
	void createShaderModule(VkShaderModule*, char*, size_t);
	void createBuffer(
		VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, 
		VkBuffer&, VkDeviceMemory&
	);
	void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize);
	uint32_t findMemoryType(uint32_t, VkMemoryPropertyFlags);
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void recreateSwapChain();
	//TODO, this is hardcoded, need a better way of handling textures
	void createTextureImage();
	void createImage(
		uint32_t, 
		uint32_t, 
		VkFormat, 
		VkImageTiling, 
		VkImageUsageFlags, 
		VkMemoryPropertyFlags, 
		VkImage&, 
		VkDeviceMemory&
	);
	void copyBufferToImage(VkBuffer, VkImage, uint32_t, uint32_t);
	void transitionImageLayout(
		VkImage, 
		VkFormat, 
		VkImageLayout, 
		VkImageLayout
	);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer);
	void createTextureImageView();
	VkImageView createImageView(VkImage, VkFormat);
	void createTextureSampler();

};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

};

#endif
