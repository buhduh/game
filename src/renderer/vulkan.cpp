#include <string>
#include <set>
#include <iostream>

#include "renderer.hpp"
#include "renderer/vulkan.hpp"

namespace renderer {

const std::vector<const char*> Vulkan::deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const size_t Vulkan::MAX_FRAMES_IN_FLIGHT(2);

//I don't think initialization lists are good for non constructor args
Vulkan::Vulkan(platform::Window* window) 
	: extensions(0) 
	, frameBufferResized(false)
	, validationLayers(0)
	, instance(VK_NULL_HANDLE)
	, window(window)
	, surface(VK_NULL_HANDLE)
	, physicalDevice(VK_NULL_HANDLE)
	, device(VK_NULL_HANDLE)
	, graphicsQueue(VK_NULL_HANDLE)
	, presentQueue(VK_NULL_HANDLE)
	, swapChain(VK_NULL_HANDLE)
	, swapChainImages(0)
	, swapChainImageFormat(VK_FORMAT_UNDEFINED)
	, swapChainExtent({0,0})
	, swapChainImageViews(0)
	, descriptorSetLayout(VK_NULL_HANDLE)
	, graphicsPipeline(VK_NULL_HANDLE)
	, pipelineLayout(VK_NULL_HANDLE)
	, swapChainFramebuffers(0)
	, commandPool(VK_NULL_HANDLE)
	, vertexBuffer(VK_NULL_HANDLE)
	, vertexBufferMemory(VK_NULL_HANDLE)
	, indexBuffer(VK_NULL_HANDLE)
	, indexBufferMemory(VK_NULL_HANDLE)
	, uniformBuffers(0)
	, uniformBuffersMemory(0)
	, descriptorPool(0)
	, descriptorSets(0)
	, commandBuffers(0)
	, imageAvailableSemaphores(0)
	, renderFinishedSemaphores(0)
	, inFlightFences(0)
	, currentFrame(0)
{
	createInstance();
	createSurface();
#ifdef DEBUG
	createDebugger();
#endif
	createPhysicalDevice();
	createLogicalDeviceAndQueue();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createFramebuffers();
	createCommandPool();
}

Vulkan::~Vulkan() {
	/*
	cleanupSwapChain();
	vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	vkDestroyBuffer(device, indexBuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
	for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
	*/
}

void Vulkan::waitIdle() {
	vkDeviceWaitIdle(device);
}

void Vulkan::cleanupSwapChain() {
	for(auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
	vkFreeCommandBuffers(
		device, commandPool, 
		static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data()
	);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	for(auto i : swapChainImageViews) {
		vkDestroyImageView(device, i, nullptr);
	}
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	for(size_t i = 0; i < swapChainImages.size(); ++i) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}

void Vulkan::recreateSwapChain() {
	vkDeviceWaitIdle(device);
	cleanupSwapChain();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createFramebuffers();
}

void Vulkan::drawFrame() {
	vkWaitForFences(
		device, 1, &inFlightFences[currentFrame], 
		VK_TRUE, std::numeric_limits<uint64_t>::max()
	);
    uint32_t imageIndex;
    auto res = vkAcquireNextImageKHR(
		device, swapChain, std::numeric_limits<uint64_t>::max(), 
		imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex
	);
	//TODO This is ALWAYS suboptimal, i should probably fix this
	//TODO need to handle resizing
	if(res == VK_ERROR_OUT_OF_DATE_KHR) {
		//TODO should I print something?
		recreateSwapChain();
		return;
	}
	assert(res == VK_SUCCESS);

	//updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	vkResetFences(device, 1, &inFlightFences[currentFrame]);
	assert(
		vkQueueSubmit(
			graphicsQueue, 1, &submitInfo, 
			inFlightFences[currentFrame]) != VK_SUCCESS
	);
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	res = vkQueuePresentKHR(presentQueue, &presentInfo);
	if(res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || frameBufferResized) {
		frameBufferResized = false;
		recreateSwapChain();
	} else {
		assert(res == VK_SUCCESS);
	}
	vkQueueWaitIdle(presentQueue);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Vulkan::createBuffer(
	VkDeviceSize size, VkBufferUsageFlags usage, 
	VkMemoryPropertyFlags properties, VkBuffer& buffer, 
	VkDeviceMemory& bufferMemory
) {
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	assert(
		vkCreateBuffer(
			device, &bufferInfo, nullptr, 
			&buffer) == VK_SUCCESS
	);
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(
		memRequirements.memoryTypeBits, properties
	);
	assert(
		vkAllocateMemory(
			device, &allocInfo, nullptr, 
			&bufferMemory) == VK_SUCCESS
	);
	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t Vulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if(
			(typeFilter & (1 << i)) && 
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties
			) {
			return i;
		}
	}
	//this is kinda awful, but the code should never get here
	//could not find suitable memory
	assert(true);
	return UINT32_MAX;
}

void Vulkan::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	vkEndCommandBuffer(commandBuffer);
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);
	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void Vulkan::createCommandPool() {
	uint32_t qFamilyIndex = getQueueFamilyIndex(physicalDevice);
	assert(qFamilyIndex != UINT32_MAX);
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = qFamilyIndex;
	poolInfo.flags = 0; // Optional
	assert(
		vkCreateCommandPool(
			device, &poolInfo, 
			nullptr, &commandPool) == VK_SUCCESS
	);
}

void Vulkan::createFramebuffers() {
	assert(swapChainImageViews.size());
	swapChainFramebuffers.resize(swapChainImageViews.size());
	for(size_t i = 0; i < swapChainImageViews.size(); ++i) {
		VkImageView attachments[] = {
			swapChainImageViews[i]
		};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
    	assert(
			vkCreateFramebuffer(
				device, &framebufferInfo, nullptr, 
				&swapChainFramebuffers[i]) == VK_SUCCESS
		);
	}
}

void Vulkan::createShaderModule(VkShaderModule* module, char* buffer, size_t shaderFSize) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderFSize;
	createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer);
	VkResult res = vkCreateShaderModule(device, &createInfo, nullptr, module) ;
	assert(res == VK_SUCCESS);
}

void Vulkan::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	VkResult result = vkCreateDescriptorSetLayout(
		device, &layoutInfo, nullptr, 
		&descriptorSetLayout
	);
	assert(result == VK_SUCCESS);
}

void Vulkan::createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;
	VkResult result = vkCreateRenderPass(
		device, &renderPassInfo, nullptr, 
		&renderPass
	);
	assert(result == VK_SUCCESS);
}

void Vulkan::createImageViews() {
	assert(swapChain != VK_NULL_HANDLE);
	assert(swapChainImages.size() > 0);
	assert(swapChainImageFormat != VK_FORMAT_UNDEFINED);
	swapChainImageViews.resize(swapChainImages.size());
	//TODO i have no idea if setting 
	for(uint32_t i = 0; i < swapChainImages.size(); ++i) {
		VkImageViewCreateInfo cInfo = {};
		cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		cInfo.image = swapChainImages[i];
		cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		cInfo.format = swapChainImageFormat;
		cInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		cInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		cInfo.subresourceRange.baseMipLevel = 0;
		cInfo.subresourceRange.levelCount = 1;
		cInfo.subresourceRange.baseArrayLayer = 0;
		cInfo.subresourceRange.layerCount = 1;
		VkResult result = vkCreateImageView(
			device, &cInfo, nullptr, 
			&swapChainImageViews[i]
		);
		assert(result == VK_SUCCESS);
	}
}


VkSurfaceFormatKHR Vulkan::chooseSwapSurfaceFormat(
	const std::vector<VkSurfaceFormatKHR> availableFormats) {
	for(const auto& a : availableFormats) {
		if(a.format == VK_FORMAT_B8G8R8A8_UNORM && 
			a.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return a;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR Vulkan::chooseSwapPresentMode(
	const std::vector<VkPresentModeKHR> availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
	for(const auto& a : availablePresentModes) {
        if(a == VK_PRESENT_MODE_MAILBOX_KHR) {
            return a;
        } else if(a == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = a;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan::chooseSwapExtent(
	const VkSurfaceCapabilitiesKHR& capabilities) {
	    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
        VkExtent2D actualExtent = {
			static_cast<uint32_t>(width), 
			static_cast<uint32_t>(height)
		};
        actualExtent.width = std::max(
			capabilities.minImageExtent.width, 
			std::min(capabilities.maxImageExtent.width, actualExtent.width)
		);
        actualExtent.height = std::max(
			capabilities.minImageExtent.height, 
			std::min(capabilities.maxImageExtent.height, actualExtent.height)
		);
        return actualExtent;
    }
}

void Vulkan::createSwapChain() {
	assert(physicalDevice != VK_NULL_HANDLE);
	assert(surface != VK_NULL_HANDLE);
	SwapChainSupportDetails details = 
		querySwapChainSupport(physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.formats);		
	VkPresentModeKHR presentMode = chooseSwapPresentMode(details.presentModes);
	VkExtent2D extent = chooseSwapExtent(details.capabilities);
	uint32_t imageCount = details.capabilities.minImageCount + 1;
	if(
		details.capabilities.maxImageCount && 
		imageCount > details.capabilities.maxImageCount
	) {
		imageCount = details.capabilities.maxImageCount;
	}
	VkSwapchainCreateInfoKHR cInfo = {};
	cInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	cInfo.surface = surface;
	cInfo.minImageCount = imageCount;
	cInfo.imageFormat = surfaceFormat.format;
	cInfo.imageColorSpace = surfaceFormat.colorSpace;
	cInfo.imageExtent = extent;
	cInfo.imageArrayLayers = 1;
	cInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//NOTE this could change if graphicsFamily != presentFamily
	cInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	cInfo.queueFamilyIndexCount = 0;
	cInfo.pQueueFamilyIndices = nullptr;
	cInfo.preTransform = details.capabilities.currentTransform;
	cInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	cInfo.presentMode = presentMode;
	cInfo.clipped = VK_TRUE;
	cInfo.oldSwapchain = VK_NULL_HANDLE;
	VkResult result = vkCreateSwapchainKHR(device, &cInfo, nullptr, &swapChain);
	assert(result == VK_SUCCESS);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void Vulkan::createLogicalDeviceAndQueue() {
	assert(physicalDevice != VK_NULL_HANDLE);
	uint32_t qFamilyIndex  = getQueueFamilyIndex(physicalDevice);
	assert(qFamilyIndex != UINT32_MAX);
	//I only need one, I think... My selection logic isn't as
	//sophisticated as the tutorials, and that mostly only uses one too
	//see https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = qFamilyIndex;
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo dCreateInfo = {};
	dCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	dCreateInfo.pNext = nullptr;
	dCreateInfo.queueCreateInfoCount = 1;
	dCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	VkPhysicalDeviceFeatures features = {};
	dCreateInfo.pEnabledFeatures = &features;
	dCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	dCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	VkResult result = vkCreateDevice(
		physicalDevice, &dCreateInfo, 
		nullptr, &device
	);
	assert(result == VK_SUCCESS);
	assert(device != VK_NULL_HANDLE);
	//TODO, these are the same, queue selection will need to be revisited for more
	//sophistication
	vkGetDeviceQueue(device, qFamilyIndex, 0, &graphicsQueue);
	vkGetDeviceQueue(device, qFamilyIndex, 0, &presentQueue);
	assert(graphicsQueue != VK_NULL_HANDLE && presentQueue != VK_NULL_HANDLE);
}

bool Vulkan::checkDeviceExtensionSupport(VkPhysicalDevice pDevice) {
   uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(
		pDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(
		pDevice, nullptr, &extensionCount, availableExtensions.data());
	//this sucks!!!!
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
    for(const auto& extension : availableExtensions) {
		if(requiredExtensions.empty()) return true;
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

uint32_t Vulkan::getQueueFamilyIndex(VkPhysicalDevice pDevice) {
	static uint32_t index = UINT32_MAX;
	//no need to keep running through this
	if(index < UINT32_MAX) {
		return index;
	}
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(
		pDevice, &queueFamilyCount, nullptr);
	//can be more than one physical device,
	//let the caller determine whether proceed.
	if(!queueFamilyCount) {
		return index;
	}
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
		pDevice, &queueFamilyCount, queueFamilies.data());
	for(uint32_t i = 0; i < queueFamilyCount; ++i) {
		auto q = queueFamilies[i];
		VkBool32 presentSupport = false;
		auto res = vkGetPhysicalDeviceSurfaceSupportKHR(
			pDevice, i, surface, &presentSupport
		);
		if(res != VK_SUCCESS) continue;
		if(q.queueCount && q.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
			index = i;
			break;
		}
	}
	return index;
}

//NOTE, this might throw a fatal error when killing the program isn't appropriate...
SwapChainSupportDetails Vulkan::querySwapChainSupport(VkPhysicalDevice pDevice) {
	SwapChainSupportDetails details;
	assert(surface != VK_NULL_HANDLE);
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		pDevice, surface, &details.capabilities);
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(
		pDevice, surface, &formatCount, nullptr);
	assert(formatCount > 0);
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
		pDevice, surface, &formatCount, details.formats.data()
	);
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		pDevice, surface, &presentModeCount, nullptr
	);
	assert(presentModeCount > 0);
	details.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(
		pDevice, surface, &presentModeCount, details.presentModes.data()
	);
    return details;
}

void Vulkan::selectSuitableDevice(const std::vector<VkPhysicalDevice> devices) {
	for(auto d : devices) {
		if(getQueueFamilyIndex(d) == UINT32_MAX) continue;
		if(!checkDeviceExtensionSupport(d)) continue;
		SwapChainSupportDetails swapChainSupport = 
			querySwapChainSupport(d);
		if(
			swapChainSupport.formats.empty() ||
			swapChainSupport.presentModes.empty()) {
			continue;
		}
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(d, &deviceProperties);
		//Selecting a descrete GPU is good enough for now
		if(deviceProperties.deviceType & VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			physicalDevice = d;
			break;
		}
	}
}

void Vulkan::createPhysicalDevice() {
	uint32_t dCount = 0;
	vkEnumeratePhysicalDevices(instance, &dCount, nullptr);
	assert(dCount > 0);
	std::vector<VkPhysicalDevice> devices(dCount);
	vkEnumeratePhysicalDevices(instance, &dCount, devices.data());
	selectSuitableDevice(devices);
	assert(physicalDevice != VK_NULL_HANDLE);
}

VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	void* pUserData
) {
	std::string severity;
	if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		severity.assign("VERBOSE");
	} else if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		severity.assign("INFO");
	} else if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		severity.assign("WARNING");
	} else if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		severity.assign("ERROR");
	}
	std::string type;
	if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
		type.assign("GENERAL");
	} else if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
		type.assign("VALIDATION");
	} else if(messageSeverity & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
		type.assign("PERFORMANCE");
	}
	//no idea how long this would need to be...
	char buff[500];
	const char* fmt = "validation layer: [%s] [%s]: %s";
	sprintf(buff, fmt, severity.c_str(), type.c_str(), pCallbackData->pMessage);
	//do i really want to do this?
	std::cout << buff << std::endl;
	return VK_FALSE;
}

void Vulkan::createDebugger() {
	VkDebugUtilsMessengerCreateInfoEXT cInfo = {};
	cInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	cInfo.pNext = nullptr;
	cInfo.flags = 0;
	cInfo.messageSeverity = 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
		//VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	cInfo.messageType = 
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT; 
	cInfo.pfnUserCallback = debugCallback;
	auto vkCreateDebugUtilsMessengerEXT = 
		(PFN_vkCreateDebugUtilsMessengerEXT) 
			vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	auto res = vkCreateDebugUtilsMessengerEXT(
		instance, &cInfo, nullptr, &debugger);
	assert(res == VK_SUCCESS);
}

void Vulkan::createSurface() {
	assert(instance != VK_NULL_HANDLE);
	//don't know if this modifies window or what....
	VkResult result = glfwCreateWindowSurface(
		instance, window, nullptr, &surface
	);
	assert(result == VK_SUCCESS);
	assert(surface != VK_NULL_HANDLE);
}

void Vulkan::setExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
	//required for layer callbacks
#ifdef DEBUG
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
}

void Vulkan::setValidationLayers() {
#ifdef DEBUG
	validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};
#endif
}

void Vulkan::createInstance() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Drone";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	setExtensions();
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

	setValidationLayers();
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	assert(result == VK_SUCCESS && instance != VK_NULL_HANDLE);
}

};
