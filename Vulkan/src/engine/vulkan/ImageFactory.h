#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class PhysicalDevice;
class Buffer;
class CommandBuffer;
class CommandPool;
class SwapChain;
class ImageView;
class Image;
class Device;


class ImageFactory
{
public:

	Image* imageColor;
	Image* imageDepth;
	Image* imageTexture;

	// Multisampling (MSAA)
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	// Mipmaps
	uint32_t mipLevels;


public:

	ImageFactory();
	~ImageFactory();

	void cleanUp(VkDevice device);

	void transitionImageLayout(Device* device, CommandPool* commandPool, VkImage image, VkFormat imageFormat,
		VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	// Texture mapping
	void createTextureImage(const char* texFilepath, Device* device, PhysicalDevice* physicalDevice,
		CommandPool* commandPool);

	// Multisampling (MSAA)
	void createColorResources(VkDevice device, PhysicalDevice* physicalDevice, SwapChain* swapChain);

	// Depth resources
	void createDepthResources(Device* device, PhysicalDevice* physicalDevice, SwapChain* swapChain, CommandPool* commandPool);

	void createTextureImageView(VkDevice device);

	VkFormat findSupportedFormat(VkPhysicalDevice hPhysicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat(VkPhysicalDevice hPhysicalDevice);

	void generateMipmaps(VkPhysicalDevice hPhysicalDevice, Device* device, CommandPool* commandPool,
		VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

};
