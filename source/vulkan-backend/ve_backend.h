//
// Created by hlahm on 2021-10-04.
//

#ifndef VULKANENGINE2_VE_BACKEND_H
#define VULKANENGINE2_VE_BACKEND_H

#include <stdexcpt.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <vector>

//imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

//stb
#include "../../external/stb_image.h"

#include "../window/ve_window.cpp"
#include "ve_validation_layer.cpp"
#include "../utilities/utilities.h"
#include "../utilities/shapes.h"


enum { MAX_INSTANCE_EXTENSIONS = 64, MAX_DEVICE_EXTENSIONS = 64, MAX_SURFACE_FORMATS = 64, MAX_PRESENTATION_MODE = 12 , MAX_IMAGE = 3, MAX_FRAME_DRAWS = 3, MAX_PIPELINES = 3, MAX_TEXTURES = 100};

static int currentFrame = 0;
static glm::vec3 mouseInit{}, mouseCurr{};

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

struct devices{
    VkPhysicalDevice physicalDevice;
    VkDevice device;
};

struct QueueFamilyIndices{
    int graphicsFamily;
    int presentationFamily;
};

struct SwapChainDetails{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;               //surface properties (image size etc...)
    VkSurfaceFormatKHR* surfaceFormat;      //surface formats (RGBA etc...)
    uint32_t surfaceFormat_size;
    VkPresentModeKHR* presentationModes; //surface formats
    uint32_t presentationModes_size;
};

struct ImageInfo{
    VkSurfaceFormatKHR surfaceFormat;
    VkExtent2D extent;
};

struct SwapChainImage{
    VkImage image;
    VkImageView imageView;
};

struct DepthBufferImage{
    VkImage image;
    VkImageView imageView;
    VkDeviceMemory imageMemory;
};

struct Pools{
    VkCommandPool graphicsCommandPool;
    VkCommandPool transferCommandPool;
};

struct Sync{
    VkSemaphore* imageAvailable;
    VkSemaphore* renderFinished;
    VkFence* drawFences;
};

struct Descriptors{
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorSetLayout samplerSetLayout;
    VkPushConstantRange pushConstantRange; //describes the size of the data to be passed through

    VkBuffer* uniformBuffer;
    VkDeviceMemory* uniformBufferMemory;

    VkBuffer* light_UB;
    VkDeviceMemory* light_UBM;

    VkDescriptorPool imGuiDescriptorPool;
    VkDescriptorPool descriptorPool;
    VkDescriptorPool samplerDescriptorPool;

    VkDescriptorSet* descriptorSet;
    VkDescriptorSet* samplerDescriptorSet;
    int num_texture_descriptor_set;
};

struct Texture{
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;

    VkImage normTextureImage;
    VkDeviceMemory normTextureImageMemory;
    VkImageView normTextureImageView;
};

struct Ve_backend{
    VkInstance instance;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue transferQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;

    VkPipelineLayout* pipelineLayout;
    VkPipeline* graphicsPipeline;
    uint32_t num_pipelines;

    VkRenderPass renderPass;
    struct devices mainDevices;
    struct QueueFamilyIndices queueFamilyIndices;
    struct SwapChainDetails swapChainDetails;
    struct Sync sync;

    //these are all the same size;
    //commandBuffer[1] will only ever output to swapChainFrameBuffer[1], which will only ever use swapChainImage[1]
    struct SwapChainImage* swapChainImages;
    struct DepthBufferImage depthBufferImages;
    VkFramebuffer* swapChainFrameBuffers;
    VkCommandBuffer* commandBuffers;

    //descriptors
    struct Descriptors descriptors;

    //for texture
    VkSampler textureSampler;

    struct ImageInfo imageInfo;
    struct Pools pools;
    uint32_t num_swapchain_images;
};

#endif //VULKANENGINE2_VE_BACKEND_H
