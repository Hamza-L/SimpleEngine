//
// Created by hlahm on 2021-10-04.
//

#include "ve_backend.h"


static struct Ve_backend vulkanBE = {};
static struct Texture* allTextures = {};
static int num_textures = 0;

static const char* instance_extentions[MAX_INSTANCE_EXTENSIONS];
static uint32_t num_instance_extensions;
static const char* deviceExtensions[MAX_DEVICE_EXTENSIONS] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
static uint32_t num_device_extensions = 1;
static VkPhysicalDeviceFeatures device_features = {.fillModeNonSolid = VK_TRUE, .samplerAnisotropy = VK_TRUE};

static void initVulkanBE(struct Ve_backend vulkanBackend){
    vulkanBE.queueFamilyIndices.graphicsFamily = -1;
    vulkanBE.queueFamilyIndices.presentationFamily = -1;
    vulkanBE.swapChainDetails.surfaceFormat = (VkSurfaceFormatKHR*)calloc(MAX_SURFACE_FORMATS,sizeof(VkSurfaceFormatKHR));
    vulkanBE.swapChainDetails.presentationModes = (VkPresentModeKHR*)calloc(MAX_PRESENTATION_MODE,sizeof(VkPresentModeKHR));
    vulkanBE.swapChainImages = (struct SwapChainImage*)(calloc(MAX_IMAGE, sizeof(struct SwapChainImage)));
    vulkanBE.graphicsPipeline = (VkPipeline*)(calloc(MAX_PIPELINES, sizeof(VkPipeline)));
    vulkanBE.pipelineLayout = (VkPipelineLayout*)(calloc(MAX_PIPELINES, sizeof(VkPipelineLayout)));
    allTextures = (struct Texture*)calloc(MAX_TEXTURES, sizeof(struct Texture));
    vulkanBE.descriptors.samplerDescriptorSet = (VkDescriptorSet*)realloc(vulkanBE.descriptors.samplerDescriptorSet, MAX_TEXTURES * sizeof(VkDescriptorSet));
}

static void cleanUpVulkanBE(){
    free(vulkanBE.swapChainDetails.surfaceFormat);
    free(vulkanBE.swapChainDetails.presentationModes);
}

static void getQueueFamilies(VkPhysicalDevice physicalDevice)
{

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    VkQueueFamilyProperties queueFamilyList[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyList);

    //Go through each queue family and check if it has at least 1 of the required types of queue
    for(int i = 0; i < queueFamilyCount; i++){
        if(queueFamilyList[i].queueCount > 0 && queueFamilyList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            vulkanBE.queueFamilyIndices.graphicsFamily = i; //if queue family is appropriate, save it.
        }

        VkBool32 presentationSupport = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, vulkanBE.surface, &presentationSupport);

        if(queueFamilyList[i].queueCount > 0 && presentationSupport){
            vulkanBE.queueFamilyIndices.presentationFamily = i;
        }

        if(vulkanBE.queueFamilyIndices.graphicsFamily >= 0 && vulkanBE.queueFamilyIndices.presentationFamily >= 0){
            break;
        }
    }

}

static int checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const char ** checkExtensions, uint32_t numExtentions){
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    //create a list of VkExtensionProperties using count;
    VkExtensionProperties extensions[extensionCount];
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions);

    //check if extentions are in list of available extentions
    for(int i = 0; i < numExtentions; i++){
        int hasExtensions = 0;
        for(int j = 0; j < extensionCount; j++){
            if(strcmp(checkExtensions[i],extensions[j].extensionName) == 0){
                hasExtensions = 1;
                break;
            }
        }

        if (!hasExtensions){
            return 0;
        }
    }

    return 1;
}

static void getSwapChainDetails(VkPhysicalDevice physicalDevice)
{
    //get surface capabilities for the given surface on the given physical device
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, vulkanBE.surface, &vulkanBE.swapChainDetails.surfaceCapabilities);

    //get formats
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vulkanBE.surface, &formatCount, nullptr);
    if (formatCount != 0){
        vulkanBE.swapChainDetails.surfaceFormat = (VkSurfaceFormatKHR*)realloc(vulkanBE.swapChainDetails.surfaceFormat ,formatCount * sizeof(VkSurfaceFormatKHR));
        vulkanBE.swapChainDetails.surfaceFormat_size = formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vulkanBE.surface, &formatCount , vulkanBE.swapChainDetails.surfaceFormat);
    }

    //get presentationmode
    uint32_t presentationCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vulkanBE.surface, &presentationCount, nullptr);
    if (presentationCount != 0){
        vulkanBE.swapChainDetails.presentationModes = (VkPresentModeKHR*)realloc(vulkanBE.swapChainDetails.presentationModes, (presentationCount) * sizeof(VkPresentModeKHR));
        vulkanBE.swapChainDetails.presentationModes_size = presentationCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vulkanBE.surface, &presentationCount , vulkanBE.swapChainDetails.presentationModes);
    }
}

static int checkIfDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    // information about the device itself
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    // information about what device can do.
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    getQueueFamilies(physicalDevice);


    int queueSupported = vulkanBE.queueFamilyIndices.graphicsFamily >= 0 && vulkanBE.queueFamilyIndices.presentationFamily >= 0;
    int extensionSupported = checkDeviceExtensionSupport(physicalDevice, deviceExtensions, num_device_extensions);

    if(!extensionSupported){
        return 0;
    }

    getSwapChainDetails(physicalDevice);

    int swapchainSupported = vulkanBE.swapChainDetails.surfaceFormat != nullptr && vulkanBE.swapChainDetails.presentationModes != nullptr;

    return queueSupported && swapchainSupported && deviceFeatures.samplerAnisotropy;
}

static void getPhysicalDevice()
{
    //enumerate physical device the VkInstance can access.
    uint32_t numDevice = 0;
    vkEnumeratePhysicalDevices(vulkanBE.instance, &numDevice, nullptr);

    if (numDevice == 0){
        printf("can't find GPU that support vulkan instance!");
    }

    //get list of physical Devices
    VkPhysicalDevice deviceList[numDevice];
    vkEnumeratePhysicalDevices(vulkanBE.instance, &numDevice, deviceList);

    VkPhysicalDeviceProperties deviceProperties;

    vkGetPhysicalDeviceProperties(deviceList[0], &deviceProperties);

    for(int i = 0; i < numDevice; i++){
        if(checkIfDeviceSuitable(deviceList[i])){
            printf("Physical Device Used: %s\n", deviceProperties.deviceName);
            vulkanBE.mainDevices.physicalDevice = deviceList[0];
        }
    }

}

static int checkInstanceExtensionSupport(const char ** checkExtensions, uint32_t numExtentions)
{
    uint32_t extentionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extentionCount, nullptr);

    //create a list of VkExtensionProperties using count;
    VkExtensionProperties extensions[extentionCount];
    vkEnumerateInstanceExtensionProperties(nullptr, &extentionCount, extensions);

    //check if extentions are in list of available extentions
    for(int i = 0; i < numExtentions; i++){
        int hasExtensions = 0;
        for(int j = 0; j < extentionCount; j++){
            if(strcmp(checkExtensions[i],extensions[j].extensionName) == 0){
                hasExtensions = 1;
                break;
            }
        }

        if (!hasExtensions){
            return 0;
        }
    }

    return 1;
}

static void createInstance()
{

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        printf("validation layers requested, but not available!");
    }

    //information about the application itself. This does not affect the program and is only for dev convenience.
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VE_App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "VulkanEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    uint32_t glfwExtensionCount = 0; //GLFW may require many extension
    const char** glfwExtensions; //Extensions passed as array of array of char.

    //get GLFW Extensions
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for(size_t i = 0; i < glfwExtensionCount; i++){
        instance_extentions[i] = glfwExtensions[i];
        num_instance_extensions++;
    }

    if(enableValidationLayers){
        instance_extentions[num_instance_extensions++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    //check if the instance extentions are supported
    if(!checkInstanceExtensionSupport(instance_extentions, num_instance_extensions)){
        printf("not all extensions are supported!");
    }

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = num_instance_extensions;
    createInfo.ppEnabledExtensionNames = instance_extentions;
    createInfo.ppEnabledLayerNames = nullptr;

    //enable validation layer
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = numValidationLayer;
        createInfo.ppEnabledLayerNames = validationLayers;

        populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    //create instance
    VkResult result = vkCreateInstance(&createInfo,nullptr,&vulkanBE.instance);
    if(result != VK_SUCCESS){
        printf("Instance creation was unsuccessful");
    }
}

static void createLogicalDevice()
{

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.enabledExtensionCount = num_device_extensions; //number of LOGICAL device extensions
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
    deviceCreateInfo.pEnabledFeatures = &device_features;

    //Queue the logical device needs to create and info to do so.
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = vulkanBE.queueFamilyIndices.graphicsFamily;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority; //if we have multiple queues running at the same time, which one has priority. 1 is highest and low is lowers.

    if(vulkanBE.queueFamilyIndices.graphicsFamily != vulkanBE.queueFamilyIndices.presentationFamily)
    {
        VkDeviceQueueCreateInfo queueCreateInfo2 = {};
        queueCreateInfo2.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo2.queueFamilyIndex = vulkanBE.queueFamilyIndices.presentationFamily;
        queueCreateInfo2.queueCount = 1;
        queueCreateInfo2.pQueuePriorities = &queuePriority;

        VkDeviceQueueCreateInfo queuesInfos[2] = {queueCreateInfo,queueCreateInfo2};

        //information to create logical device


        deviceCreateInfo.queueCreateInfoCount = (uint32_t)2;
        deviceCreateInfo.pQueueCreateInfos = queuesInfos;

        VkResult result = vkCreateDevice(vulkanBE.mainDevices.physicalDevice, &deviceCreateInfo, nullptr, &vulkanBE.mainDevices.device);
        if (result != VK_SUCCESS){
            printf("Failed to create logical device");
        }

        //Queue are created at the same time as device. now we want to get queue handles.
        vkGetDeviceQueue(vulkanBE.mainDevices.device, vulkanBE.queueFamilyIndices.graphicsFamily, 0, &vulkanBE.graphicsQueue);
        vkGetDeviceQueue(vulkanBE.mainDevices.device, vulkanBE.queueFamilyIndices.presentationFamily, 0, &vulkanBE.presentQueue);

    } else
    {
        //information to create logical device
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;


        VkResult result = vkCreateDevice(vulkanBE.mainDevices.physicalDevice, &deviceCreateInfo, nullptr, &vulkanBE.mainDevices.device);
        if (result != VK_SUCCESS){
            printf("Failed to create logical device");
        }

        //Queue are created at the same time as device. now we want to get queue handles.
        vkGetDeviceQueue(vulkanBE.mainDevices.device, vulkanBE.queueFamilyIndices.graphicsFamily, 0, &vulkanBE.graphicsQueue);
        vkGetDeviceQueue(vulkanBE.mainDevices.device, vulkanBE.queueFamilyIndices.graphicsFamily, 0, &vulkanBE.presentQueue);
    }

    //as per vulkan standard, transferqueue is always graphicsqueue
    vulkanBE.transferQueue = vulkanBE.graphicsQueue;

}

static void createSurface()
{
    //creates a surface create info struct, runs the surface create function and returns a VK_Result. It is essentially a wrapper function
    VkResult result = glfwCreateWindowSurface(vulkanBE.instance, ve_window.window, nullptr, &vulkanBE.surface);
    if (result != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create Surface!\n");
        exit(1);
    }

}

//format we will choose is          VK_FORMAT_R8G8B8A8_UNORM
//colourspace we will choose is     VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
static VkSurfaceFormatKHR chooseSwapchainFormat()
{
    VkSurfaceFormatKHR bestFormat = {};
    if(vulkanBE.swapChainDetails.surfaceFormat_size == 1 && vulkanBE.swapChainDetails.surfaceFormat[0].format == VK_FORMAT_UNDEFINED )
    {
        bestFormat.format = VK_FORMAT_R8G8B8A8_UNORM;
        bestFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return bestFormat;
    }

    for(int i = 0; i < vulkanBE.swapChainDetails.surfaceFormat_size; i++){
        if(vulkanBE.swapChainDetails.surfaceFormat[i].format == VK_FORMAT_R8G8B8A8_UNORM ||
        vulkanBE.swapChainDetails.surfaceFormat[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
        vulkanBE.swapChainDetails.surfaceFormat[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return vulkanBE.swapChainDetails.surfaceFormat[i];
        }
    }

    return vulkanBE.swapChainDetails.surfaceFormat[0];
}

static VkPresentModeKHR chooseSwapchainPresentMode()
{

    //look for mailbox Presentation mode. if not found, use FIFO
    for(int i = 0; i < vulkanBE.swapChainDetails.presentationModes_size; i++){
        if(vulkanBE.swapChainDetails.presentationModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return vulkanBE.swapChainDetails.presentationModes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapChainExtent()
{
    if(vulkanBE.swapChainDetails.surfaceCapabilities.currentExtent.width != UINT32_MAX)
    {
        //if value is fixed, we return it.
        return vulkanBE.swapChainDetails.surfaceCapabilities.currentExtent;
    }
    else
    {   //if the value can vary, we need to set it manually.
        int width,height;
        glfwGetFramebufferSize(ve_window.window, &width, &height);

        VkExtent2D newExtent = {};
        newExtent.width = (uint32_t) width;
        newExtent.height = (uint32_t) height;

        //surface also defines max and min so we need to make sure we are within boundaries
        if(newExtent.width > vulkanBE.swapChainDetails.surfaceCapabilities.maxImageExtent.width || newExtent.width < vulkanBE.swapChainDetails.surfaceCapabilities.minImageExtent.width)
        {
            newExtent.width = vulkanBE.swapChainDetails.surfaceCapabilities.maxImageExtent.width;
        }

        if(newExtent.height > vulkanBE.swapChainDetails.surfaceCapabilities.maxImageExtent.height || newExtent.height < vulkanBE.swapChainDetails.surfaceCapabilities.minImageExtent.height)
        {
            newExtent.height = vulkanBE.swapChainDetails.surfaceCapabilities.maxImageExtent.height;
        }

        return newExtent;
    }
}

static VkFormat chooseSupportedFormat(VkFormat* formats, int num_formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags)
{
    //loop through the options and find compatible one
    for(size_t i = 0; i < num_formats; i++)
    {
        //get Properties for
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(vulkanBE.mainDevices.physicalDevice, formats[i], &properties);

        if(tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags)
        {
            return formats[i];
        }else if(tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & featureFlags) == featureFlags)
        {
            return formats[i];
        }
    }

    fprintf(stderr, "failed to find a matching format!");
    exit(1);
}

static uint32_t findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties){
    //Get properties of physical device memory
    VkPhysicalDeviceMemoryProperties memoryProperties = {};
    vkGetPhysicalDeviceMemoryProperties(vulkanBE.mainDevices.physicalDevice, &memoryProperties);

    for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if((allowedTypes & (1 << i)) && ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties))
        {
            return i; //this memorytype is valid so we return it.
        }
    }

    return 0;
}

static VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags flags, VkMemoryPropertyFlags propertyFlags, VkDeviceMemory *imageMemory)
{
    //1. create the image
    //2. create memory for the image

    VkImageCreateInfo imageCreateInfo = {};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = format;
    imageCreateInfo.tiling = tiling;
    imageCreateInfo.usage = flags;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT; //for multisampling
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //wether image can be shared between queues.

    VkImage image;
    VkResult result = vkCreateImage(vulkanBE.mainDevices.device, &imageCreateInfo, nullptr, &image);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create an Image\n");
        exit(1);
    }

    //create memory for image

    //get memory requirement for a type of image
    VkMemoryRequirements memoryRequirements = {};
    vkGetImageMemoryRequirements(vulkanBE.mainDevices.device, image, &memoryRequirements);

    //allocate memory using image requirement and user defined properties
    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryTypeIndex(memoryRequirements.memoryTypeBits, propertyFlags);

    result = vkAllocateMemory(vulkanBE.mainDevices.device, &memoryAllocateInfo, nullptr, imageMemory);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to allocate memory for an Image\n");
        exit(1);
    }

    //connect memory to image
    vkBindImageMemory(vulkanBE.mainDevices.device, image, *imageMemory, 0);

    return image;
}

static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlagBits aspectFlags)
{
    VkImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.pNext = nullptr;
    viewCreateInfo.flags = 0;
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.image = image;   //image to create view for
    viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;  //image type (2D, 3D etc)
    viewCreateInfo.format = format;     //format of image fata
    viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;  //allows remapping of colour values
    viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    //Sub ressources allow the view to view only part of an image
    viewCreateInfo.subresourceRange.aspectMask = aspectFlags; //which aspect of image to view (COLOUR BIT for our case)
    viewCreateInfo.subresourceRange.baseMipLevel = 0; //start mipmap level to view from
    viewCreateInfo.subresourceRange.levelCount = 1; //number of mipmap levels to view
    viewCreateInfo.subresourceRange.baseArrayLayer = 0; //start array level to view from
    viewCreateInfo.subresourceRange.layerCount = 1; //number of array levels to view

    //create image view and return it;
    VkImageView imageView;
    VkResult result = vkCreateImageView(vulkanBE.mainDevices.device, &viewCreateInfo, nullptr, &imageView);
    if(result != VK_SUCCESS){
        fprintf(stderr, "Failed to create an image view\n");
        exit(1);
    }

    return imageView;
}

static void createSwapChain()
{
    //choose best surface format
    VkSurfaceFormatKHR surfaceFormat = chooseSwapchainFormat();

    //choose best presentation mode
    VkPresentModeKHR presentMode = chooseSwapchainPresentMode();

    //chose swapchain resolution
    VkExtent2D extent = chooseSwapChainExtent();

    //how many images are in the swapChain. we will get one more than the min to allow for triple buffer.
    uint32_t maxImageCount = vulkanBE.swapChainDetails.surfaceCapabilities.minImageCount; // if 0, it is limitless.

    if(maxImageCount > vulkanBE.swapChainDetails.surfaceCapabilities.maxImageCount && maxImageCount > 0){
        maxImageCount = vulkanBE.swapChainDetails.surfaceCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.minImageCount = maxImageCount;
    swapChainCreateInfo.imageArrayLayers = 1; //number of layers for each image in the swapchain
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //what attachment images will be used. you shouldn't but depth data in swapchain.
    swapChainCreateInfo.preTransform = vulkanBE.swapChainDetails.surfaceCapabilities.currentTransform; //transform to perform on swapchain images.
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //don't do any blending of the window with external graphics (eg other windows)
    swapChainCreateInfo.clipped = VK_TRUE; //how to clip with external graphics. if a window overlaps with our window, do not render and clip.

    //if graphics and presentation families are different then swapchain must let images be shared between families;
    if(vulkanBE.queueFamilyIndices.graphicsFamily != vulkanBE.queueFamilyIndices.presentationFamily)
    {
        uint32_t queueFamilyIndices[2] = {(uint32_t)vulkanBE.queueFamilyIndices.graphicsFamily, (uint32_t)vulkanBE.queueFamilyIndices.presentationFamily};

        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE; //in case we destroy a swapchain and want to recreate it. we can use the old swapchain to quickly hand over responsibilities.
    swapChainCreateInfo.surface = vulkanBE.surface;

    VkResult result = vkCreateSwapchainKHR(vulkanBE.mainDevices.device, &swapChainCreateInfo, nullptr, &vulkanBE.swapChain);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create Swapchain\n");
        exit(1);
    }

    vulkanBE.imageInfo.surfaceFormat = surfaceFormat;
    vulkanBE.imageInfo.extent = extent;

    uint32_t swapChainImageCount;
    vkGetSwapchainImagesKHR(vulkanBE.mainDevices.device, vulkanBE.swapChain, &swapChainImageCount, nullptr);
    VkImage* images = (VkImage*)calloc(swapChainImageCount, sizeof(VkImage));
    vkGetSwapchainImagesKHR(vulkanBE.mainDevices.device, vulkanBE.swapChain, &swapChainImageCount, images);

    vulkanBE.swapChainImages = (struct SwapChainImage*)realloc(vulkanBE.swapChainImages, swapChainImageCount * sizeof(struct SwapChainImage));
    vulkanBE.num_swapchain_images = swapChainImageCount;

    for(int i = 0; i<swapChainImageCount; i++)
    {
        //create image
        vulkanBE.swapChainImages[i].image = images[i];

        //create image view here
        vulkanBE.swapChainImages[i].imageView = createImageView(images[i], surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

static VkShaderModule createShaderModule(struct ShaderCode* shaderCode)
{
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = shaderCode->code_size;
    shaderModuleCreateInfo.pCode = (const uint32_t *)shaderCode->code;
    shaderModuleCreateInfo.pNext = nullptr;
    shaderModuleCreateInfo.flags = 0;

    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(vulkanBE.mainDevices.device, &shaderModuleCreateInfo, nullptr, &shaderModule);
    if(result != VK_SUCCESS){
        fprintf(stderr, "failed to create shader module\n");
        exit(1);
    }
    return shaderModule;
}

static void createRenderPass()
{
    //----ATTACHEMENT
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = vulkanBE.imageInfo.surfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; //number of sample to write for MultiSampling
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //what to do with attachment before rendering
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //we want to present it so we have to store it somewhere.
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //describes what to do with stencil before rendering
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //we do not care once again.

    //framebuffer data will be stored as image, but images can be given different data layouts to give optimal use for certain operations.
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //image layout before the render pass starts, before all subpass start.
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  //image data layout after render pass. this is at the end of all subpass.

    VkFormat formats_to_check[] = {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT};

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = chooseSupportedFormat(formats_to_check, 3, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT; //number of sample to write for MultiSampling
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //what to do with attachment before rendering
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //----REFERENCES
    //attachment references uses attachment index that refers to index in the attachment list passed to renderpasscreateinfo
    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0; //use the first attachment on that list of color attachment passed unto the renderpass.
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; //this is the subpass layout.

    VkAttachmentReference depthAttachmentReference = {};
    depthAttachmentReference.attachment = 1; //use the first attachment on that list of color attachment passed unto the renderpass.
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; //this is the subpass layout.

    //Subpass info
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //we want it to bind to what type of pipeline? a graphics pipeline.
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentReference; //we cannot pass the color attachment directly. we have to pass a color attachment reference.
    subpass.pDepthStencilAttachment = &depthAttachmentReference; //there si only one depth attachment possible for one subpass.

    //need to determine when layout transitions occur using subpass dependencies. these create implicit layout transitions.
    VkSubpassDependency subpassDependency[2];
    //conversion from VK_IMAGE_LAYOUT_UNDEFINED TO VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL. The transition is BETWEEN src and dst
    //-----------------------------------------------conversion has to happen after we go to subpass external and attempt to read form it.
    subpassDependency[0].srcSubpass = VK_SUBPASS_EXTERNAL; //anything outside of our subpass
    subpassDependency[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; //the end of the pipeline; what has to happen first
    subpassDependency[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT; //it has to be read before converting it to the color attachment optimal
    //-----------------------------------------------conversion has to happen before we got to the first subpass and we can read and write from it.
    subpassDependency[0].dstSubpass = 0; //the first subpass.
    subpassDependency[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    subpassDependency[0].dependencyFlags = 0;

    //conversion from VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL TO VK_IMAGE_LAYOUT_PRESENT_SRC_KHR. The transition is BETWEEN src and dst
    //-----------------------------------------------conversion has to happen after ...
    subpassDependency[1].srcSubpass = 0;
    subpassDependency[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    //-----------------------------------------------conversion has to happen before ...
    subpassDependency[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    subpassDependency[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    subpassDependency[1].dependencyFlags = 0;

    VkAttachmentDescription attachmentDescriptions[] = {colorAttachment, depthAttachment};

    //Renderpass INFO
    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 2;
    renderPassCreateInfo.pAttachments = attachmentDescriptions;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = 2;
    renderPassCreateInfo.pDependencies = subpassDependency;


    VkResult result = vkCreateRenderPass(vulkanBE.mainDevices.device, &renderPassCreateInfo, nullptr, &vulkanBE.renderPass);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create a renderpass\n");
        exit(1);
    }

}

static void createDescriptorPool()
{
    //sampler descriptor pool
    VkDescriptorPoolSize samplerPoolSize = {};
    samplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerPoolSize.descriptorCount = (uint32_t)MAX_TEXTURES;

    VkDescriptorPoolCreateInfo samplerPoolCreateInfo = {};
    samplerPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    samplerPoolCreateInfo.maxSets = (uint32_t)MAX_TEXTURES; //maximum number of descriptor sets that can be created form the pool;
    samplerPoolCreateInfo.poolSizeCount = 1;
    samplerPoolCreateInfo.pPoolSizes = &samplerPoolSize;

    //create Descriptor Pool
    VkResult result = vkCreateDescriptorPool(vulkanBE.mainDevices.device, &samplerPoolCreateInfo, nullptr, &vulkanBE.descriptors.samplerDescriptorPool);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create sampler descriptor pool\n");
        exit(1);
    }

    //create uniform descriptor pool
    int num_descriptors = 2;

    //type of descriptors and how many of them (not to confuse with sets)
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount =(uint32_t)MAX_IMAGE * num_descriptors; //how many descriptors do we need

    VkDescriptorPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.maxSets = (uint32_t)MAX_IMAGE * num_descriptors; //maximum number of descriptor sets that can be created form the pool;
    poolCreateInfo.poolSizeCount = 1;
    poolCreateInfo.pPoolSizes = &poolSize;

    //create Descriptor Pool
    result = vkCreateDescriptorPool(vulkanBE.mainDevices.device, &poolCreateInfo, nullptr, &vulkanBE.descriptors.descriptorPool);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create descriptor pool\n");
        exit(1);
    }

    //ImGui Descriptor Pool creation
    VkDescriptorPoolSize imGuiPoolSizes[] =
            {
                    { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                    { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                    { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
            };
    VkDescriptorPoolCreateInfo ImGuiPoolInfo = {};
    ImGuiPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    ImGuiPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    ImGuiPoolInfo.maxSets = 1000 * 11;
    ImGuiPoolInfo.poolSizeCount = (uint32_t)11;
    ImGuiPoolInfo.pPoolSizes = imGuiPoolSizes;

    VkResult err = vkCreateDescriptorPool(vulkanBE.mainDevices.device, &ImGuiPoolInfo, nullptr, &vulkanBE.descriptors.imGuiDescriptorPool);
    check_vk_result(err);
}

static void createDescriptorSet()
{
    vulkanBE.descriptors.descriptorSet = (VkDescriptorSet*)realloc(vulkanBE.descriptors.descriptorSet, MAX_IMAGE * sizeof(VkDescriptorSet));

    VkDescriptorSetLayout setLayouts[3] = {vulkanBE.descriptors.descriptorSetLayout,vulkanBE.descriptors.descriptorSetLayout,vulkanBE.descriptors.descriptorSetLayout}; //we have to have a 1 to 1 relationship between each set and setlayout

    VkDescriptorSetAllocateInfo setAllocateInfo = {};
    setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    setAllocateInfo.descriptorPool = vulkanBE.descriptors.descriptorPool;
    setAllocateInfo.descriptorSetCount = (uint32_t)MAX_IMAGE;
    setAllocateInfo.pSetLayouts = setLayouts;

    //Allocate descriptorSets
    VkResult result = vkAllocateDescriptorSets(vulkanBE.mainDevices.device, &setAllocateInfo, vulkanBE.descriptors.descriptorSet);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create descriptor sets\n");
        exit(1);
    }
}

static void createDescriptorSetLayout()
{

    //create uniform buffer descriptor set layout

    //Camera binding info
    VkDescriptorSetLayoutBinding camLayoutBinding = {};
    camLayoutBinding.binding = 0; //binding point in shader (designated by binding number in vertex shader) (set = 0, binding = 0)
    camLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //type of descriptor (uniform, dynamic, texture etc..)
    camLayoutBinding.descriptorCount = 1; //number of descriptor for binding
    camLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    camLayoutBinding.pImmutableSamplers = nullptr; //for texture. can make the sampler immutable

    //lights binding info
    VkDescriptorSetLayoutBinding lightLayoutBinding = {};
    lightLayoutBinding.binding = 1; //binding point in shader (designated by binding number in vertex shader) (set = 0, binding = 1)
    lightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //type of descriptor (uniform, dynamic, texture etc..)
    lightLayoutBinding.descriptorCount = 1; //number of descriptor for binding
    lightLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    lightLayoutBinding.pImmutableSamplers = nullptr; //for texture. can make the sampler immutable

    //all bindings
    VkDescriptorSetLayoutBinding layoutBindings[] = {camLayoutBinding, lightLayoutBinding};

    //create Descriptor Set Layout with given bindings
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount = 2;
    descriptorSetLayoutCreateInfo.pBindings = layoutBindings;

    //create descriptor set layout
    VkResult result = vkCreateDescriptorSetLayout(vulkanBE.mainDevices.device, &descriptorSetLayoutCreateInfo, nullptr, &vulkanBE.descriptors.descriptorSetLayout);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create descriptor set layout\n");
        exit(1);
    }

    //create texture sampler descriptor set layout
    //texture sampler binding info
    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 0; //binding point in shader (designated by binding number in fragment shader) (set = 1, binding = 0)
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //type of descriptor (uniform, dynamic, texture etc..)
    samplerLayoutBinding.descriptorCount = 1; //number of descriptor for binding
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr; //for texture. can make the sampler immutable

    //create sampler Descriptor Set Layout with given bindings
    VkDescriptorSetLayoutCreateInfo textureDescriptorSetLayoutCreateInfo = {};
    textureDescriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    textureDescriptorSetLayoutCreateInfo.bindingCount = 1;
    textureDescriptorSetLayoutCreateInfo.pBindings = &samplerLayoutBinding;

    //create descriptor set layout
    result = vkCreateDescriptorSetLayout(vulkanBE.mainDevices.device, &textureDescriptorSetLayoutCreateInfo, nullptr, &vulkanBE.descriptors.samplerSetLayout);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create sampler descriptor set layout\n");
        exit(1);
    }
}

static void createPushConstantRange()
{
    vulkanBE.descriptors.pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    vulkanBE.descriptors.pushConstantRange.offset = 0;
    vulkanBE.descriptors.pushConstantRange.size = sizeof(struct PushObj);
}

static void createGraphicsPipeline(VkPolygonMode flag)
{

    //read in SPIR-V Code
    struct ShaderCode vertexShaderCode = readSPRVFile("C:/Users/hlahm/Documents/gitProjects/SimpleEngine/source/shader/vert.spv");
    struct ShaderCode fragShaderCode = readSPRVFile("C:/Users/hlahm/Documents/gitProjects/SimpleEngine/source/shader/frag.spv");

    //Build shader Modules to link to graphics pipeline
    VkShaderModule vertexShaderModule = createShaderModule(&vertexShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(&fragShaderCode);

    //vertex stage create info
    VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
    vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderCreateInfo.module = vertexShaderModule;
    vertexShaderCreateInfo.pName = "main"; //shader's first function (entry point)

    //fragment stage create info
    VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderCreateInfo.module = fragShaderModule;
    fragmentShaderCreateInfo.pName = "main"; //shader's first function (entry point)

    //put shadercreateinfo in array.
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertexShaderCreateInfo, fragmentShaderCreateInfo};

    //how the data for a single vertex (position color etc) is as a whole
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;                         //can bind multiple streams of data, this defines which one. this can be seen directly on the shader.
    bindingDescription.stride = sizeof(struct Vertex);      //size of single Vertex Object
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; //draw one vertex at a time or draw the vertex of each instances at a time (VK_VERTEX_INPUT_RATE_INSTANCES).

    //how the data for an attribute is defined within a vertex
    //position attribute
    VkVertexInputAttributeDescription positionAttributeDescription = {};
    positionAttributeDescription.binding = 0;
    positionAttributeDescription.location = 0;
    positionAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT; //what a vec3 is defined as memory wise
    positionAttributeDescription.offset = offsetof(struct Vertex, position); //the offset within the structure

    VkVertexInputAttributeDescription normalAttributeDescription = {};
    normalAttributeDescription.binding = 0;
    normalAttributeDescription.location = 1;
    normalAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT; //what a vec3 is defined as memory wise
    normalAttributeDescription.offset = offsetof(struct Vertex, normal); //the offset within the structure

    VkVertexInputAttributeDescription colorAttributeDescription = {};
    colorAttributeDescription.binding = 0;
    colorAttributeDescription.location = 2;
    colorAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT; //what a vec3 is defined as memory wise
    colorAttributeDescription.offset = offsetof(struct Vertex, color); //the offset within the structure

    VkVertexInputAttributeDescription texAttributeDescription = {};
    texAttributeDescription.binding = 0;
    texAttributeDescription.location = 3;
    texAttributeDescription.format = VK_FORMAT_R32G32_SFLOAT; //what a vec3 is defined as memory wise
    texAttributeDescription.offset = offsetof(struct Vertex, texCoord); //the offset within the structure

    VkVertexInputAttributeDescription attributeDescription[] = {positionAttributeDescription, normalAttributeDescription, colorAttributeDescription, texAttributeDescription};

    //Vertex input
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
    vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription; //list of vertex binding description (data spacing and stride information).
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 4; //number of attributes.
    vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescription; //list of Vertex Binding Attribute Descriptions (data format and where to bind to/from)

    //input assembly (how to assemble the input into primitive shapes).
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; //primitive type to assemble vertices
    inputAssembly.primitiveRestartEnable = VK_FALSE; //allow overriding of strip topology to start new primitive.

    //viewport and scissor
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = (float)vulkanBE.imageInfo.extent.height;
    viewport.width = (float)vulkanBE.imageInfo.extent.width;
    viewport.height = -(float)vulkanBE.imageInfo.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    //create a scissor info struct
    VkRect2D scissor = {};
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.width = vulkanBE.imageInfo.extent.width;
    scissor.extent.height = vulkanBE.imageInfo.extent.height;

    VkPipelineViewportStateCreateInfo  viewportStateCreateInfo = {};
    viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateCreateInfo.viewportCount = 1;
    viewportStateCreateInfo.pViewports = &viewport;
    viewportStateCreateInfo.scissorCount = 1;
    viewportStateCreateInfo.pScissors = &scissor;

    //rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
    rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.depthClampEnable = VK_FALSE; //we don't want object beyond the far plane to be clipped off or clamped (needs GPU feature).
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE; //after doing all the pipeline stages, we discard the rasterization stage. we want to use the rasterizer so keep it false.
    rasterizationStateCreateInfo.polygonMode = flag;
    rasterizationStateCreateInfo.lineWidth = 1.0f;
    rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE; //whether to add depth bias to fragment to stopping shadow acne

    //multisampling
    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE; //whether multisampling shading is enabled;
    multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; //takes only one sample in the middle of that fragment

    //blending
    //blend attachment state
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
    colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachmentState.blendEnable = VK_TRUE; //enable blending.

    //blending uses the following equation (srcColorBlendFactor * newColor) colorBlendOp (dstColorBlendFactor * oldColor);
    colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    //(1*newAlpha) + (0 * oldAlpha) = newAlpha.

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.logicOpEnable = VK_FALSE; //alternative to calculation is to use logic operators
    colorBlendStateCreateInfo.attachmentCount = 1;
    colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;

    //pipeline layout (TODO: apply future descriptor set layout and push constants)
    VkDescriptorSetLayout descriptorSetLayouts[2] = {vulkanBE.descriptors.descriptorSetLayout, vulkanBE.descriptors.samplerSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 2;
    pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts;
    pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pipelineLayoutCreateInfo.pPushConstantRanges = &vulkanBE.descriptors.pushConstantRange;

    // dynamic state would be created here (something alterable via command buffer)

    //pipeline layout creation
    VkResult result = vkCreatePipelineLayout(vulkanBE.mainDevices.device, &pipelineLayoutCreateInfo, nullptr, &vulkanBE.pipelineLayout[vulkanBE.num_pipelines]);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create pipeline Layout\n");
        exit(1);
    }

    //setup depth stencil (TODO:setup depth stencil testing)
    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo = {};
    depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS; //when pixel is in front override
    depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE; //we don't want to enable stencil test

    //Graphics pipeline creation
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;  //number of shader stages
    pipelineCreateInfo.pStages = shaderStages; //list of shader stages
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo; //all the fixed function pipeline states
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
    pipelineCreateInfo.layout = vulkanBE.pipelineLayout[vulkanBE.num_pipelines]; //pipeline layout pipeline should use
    pipelineCreateInfo.renderPass = vulkanBE.renderPass; //Render pass description the pipeline is compatible with
    pipelineCreateInfo.subpass = 0; //subpass of renderpass to use with pipeline.

    //pipeline derivatives. can create multiple pipeline that device form one another for optimization.
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE; //useful for creating a pipeline from a template
    pipelineCreateInfo.basePipelineIndex = -1; //can create multiple pipelines at once. this is the index of the pipeline which the creation of pipelines should base itself on.

    result = vkCreateGraphicsPipelines(vulkanBE.mainDevices.device,VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &vulkanBE.graphicsPipeline[vulkanBE.num_pipelines]);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create graphics pipeline\n");
        exit(1);
    }
    vulkanBE.num_pipelines++;

    //destroy shader module and code
    vkDestroyShaderModule(vulkanBE.mainDevices.device, fragShaderModule, nullptr);
    vkDestroyShaderModule(vulkanBE.mainDevices.device, vertexShaderModule, nullptr);
    free(vertexShaderCode.code);
    free(fragShaderCode.code);
}

static void createDepthBufferImage()
{
    //get supported format for depth buffer
    VkFormat formats_to_check[] = {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT};
    VkFormat depthFormat = chooseSupportedFormat(formats_to_check, 3, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    //create depth buffer image
    vulkanBE.depthBufferImages.image = createImage(vulkanBE.imageInfo.extent.width,
                                                         vulkanBE.imageInfo.extent.height,
                                                         depthFormat,
                                                         VK_IMAGE_TILING_OPTIMAL,
                                                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                                         &vulkanBE.depthBufferImages.imageMemory);

    vulkanBE.depthBufferImages.imageView = createImageView(vulkanBE.depthBufferImages.image,
                                                           depthFormat,
                                                           VK_IMAGE_ASPECT_DEPTH_BIT);
}

static void createFrameBuffers()
{
    //resize framebuffer count to equal swapchain image count
    vulkanBE.swapChainFrameBuffers = (VkFramebuffer*)realloc(vulkanBE.swapChainFrameBuffers, vulkanBE.num_swapchain_images * sizeof(VkFramebuffer));

    //create a framebuffer for each swapChain images
    for(size_t i=0; i<vulkanBE.num_swapchain_images; i++)
    {
        //1:1 relationship with the renderpass attachment output.
        VkImageView attachments[] = {
                vulkanBE.swapChainImages[i].imageView,
                vulkanBE.depthBufferImages.imageView
        };

        VkFramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = vulkanBE.renderPass; //renderpass the framebuffer will be used with
        framebufferCreateInfo.attachmentCount = 2;
        framebufferCreateInfo.pAttachments = attachments; //list of attachment 1:1 with renderpass attachment
        framebufferCreateInfo.width = vulkanBE.imageInfo.extent.width; //framebuffer width
        framebufferCreateInfo.height = vulkanBE.imageInfo.extent.height; //framebuffer height
        framebufferCreateInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(vulkanBE.mainDevices.device, &framebufferCreateInfo, nullptr, &vulkanBE.swapChainFrameBuffers[i]);
        if(result != VK_SUCCESS)
        {
            fprintf(stderr, "failed to create framebuffer\n");
            exit(1);
        }

    }
}

static void createCommandPool(){
    VkCommandPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //the command buffer will be reset everytime we call vkBeginCOmmandBuffer
    poolCreateInfo.queueFamilyIndex = vulkanBE.queueFamilyIndices.graphicsFamily; //queue family that buffers from this command pool will use

    VkResult result = vkCreateCommandPool(vulkanBE.mainDevices.device, &poolCreateInfo, nullptr, &vulkanBE.pools.graphicsCommandPool);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "Failed to create commandpool\n");
        exit(1);
    }

    vulkanBE.pools.transferCommandPool = vulkanBE.pools.graphicsCommandPool;
}

static void createCommandBuffers()
{
    //resize commandBuffer count to equal swapchain image count
    vulkanBE.commandBuffers = (VkCommandBuffer*)realloc(vulkanBE.commandBuffers, vulkanBE.num_swapchain_images * sizeof(VkCommandBuffer));

    VkCommandBufferAllocateInfo cbAllocInfo = {};
    cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cbAllocInfo.commandPool = vulkanBE.pools.graphicsCommandPool;
    cbAllocInfo.commandBufferCount = vulkanBE.num_swapchain_images;
    cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; //directly submits to queue. can't be called by other buffers.
                                                            //VK_COMMAND_BUFFER_LEVEL_SECONDARY means it cannot be submitted to queue and can only be called by other commanbuffers

    //we do not create command buffers. we allocate. as soon as commandpool is destroyed, so are commandbuffers.
    VkResult result = vkAllocateCommandBuffers(vulkanBE.mainDevices.device, &cbAllocInfo, vulkanBE.commandBuffers); //allocates all the command buffers at once
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to allocate command buffers\n");
        exit(1);
    }
}

static void createSynchronization()
{
    vulkanBE.sync.imageAvailable = (VkSemaphore*)realloc(vulkanBE.sync.imageAvailable, MAX_FRAME_DRAWS * sizeof(VkSemaphore));
    vulkanBE.sync.renderFinished = (VkSemaphore*)realloc(vulkanBE.sync.renderFinished, MAX_FRAME_DRAWS * sizeof(VkSemaphore));
    vulkanBE.sync.drawFences = (VkFence*)realloc(vulkanBE.sync.drawFences, MAX_FRAME_DRAWS * sizeof(VkFence));

    //semaphore creation information
    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(size_t i=0; i<MAX_FRAME_DRAWS; i++)
    {
        if (vkCreateSemaphore(vulkanBE.mainDevices.device, &semaphoreCreateInfo, nullptr, &vulkanBE.sync.imageAvailable[i]) !=
            VK_SUCCESS ||
            vkCreateSemaphore(vulkanBE.mainDevices.device, &semaphoreCreateInfo, nullptr, &vulkanBE.sync.renderFinished[i]) !=
            VK_SUCCESS ||
            vkCreateFence(vulkanBE.mainDevices.device, &fenceCreateInfo, nullptr, &vulkanBE.sync.drawFences[i]))
        {
            fprintf(stderr, "failed to create semaphores and/or fences\n");
            exit(1);
        }
    }

}

static void createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags bufferProperties, VkBuffer* buffer, VkDeviceMemory* bufferMemory)
{
    //a buffer describes the memory. it does not include assigning memory
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = bufferSize;
    bufferCreateInfo.usage = bufferUsageFlags;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkResult result = vkCreateBuffer(vulkanBE.mainDevices.device, &bufferCreateInfo, nullptr, buffer);
    if (result != VK_SUCCESS)
    {
        fprintf(stderr,"failed to create buffer\n");
        exit(1);
    }

    //Get Buffer memory requirements
    VkMemoryRequirements memoryRequirements = {};
    vkGetBufferMemoryRequirements(vulkanBE.mainDevices.device, *buffer, &memoryRequirements);

    //Allocate memory to buffer
    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = findMemoryTypeIndex(memoryRequirements.memoryTypeBits, //index of memory type on physical device for required bit flags
                                                             bufferProperties); //the properties we want for the buffer

    result = vkAllocateMemory(vulkanBE.mainDevices.device, &memoryAllocateInfo, nullptr, bufferMemory);
    if ( result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to allocate vertex buffer memory");
        exit(1);
    }

    //allocate memory to given vertex buffer
    vkBindBufferMemory(vulkanBE.mainDevices.device, *buffer, *bufferMemory, 0);

}

static void createTextureSampler()
{
    VkSamplerCreateInfo samplerCreateInfo = {};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR; //linear interpolation between the pixels
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR; //linear interpolation between the pixels
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; //how to handle texture wrapping in the u(x) direction
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE; //it is normalized
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.maxLod = 0.0f;
    samplerCreateInfo.anisotropyEnable = VK_TRUE;
    samplerCreateInfo.maxAnisotropy = 16; //number of samples taken for anisotropy

    VkResult result = vkCreateSampler(vulkanBE.mainDevices.device, &samplerCreateInfo, nullptr, &vulkanBE.textureSampler);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to create sampler!\n");
        exit(1);
    }
}

static int createTextureDescriptor(VkImageView textureImageView)
{

    VkDescriptorSet descriptorSet;

    //allocate memory for descriptorSet
    VkDescriptorSetAllocateInfo setAllocateInfo = {};
    setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    setAllocateInfo.descriptorPool = vulkanBE.descriptors.samplerDescriptorPool;
    setAllocateInfo.descriptorSetCount = 1;
    setAllocateInfo.pSetLayouts = &vulkanBE.descriptors.samplerSetLayout;

    VkResult result = vkAllocateDescriptorSets(vulkanBE.mainDevices.device, &setAllocateInfo, &descriptorSet);
    if(result != VK_SUCCESS)
    {
        fprintf(stderr, "failed to allocate combine image sampler descriptor set\n");
        exit(1);
    }

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; //what is the image layout when in use
    imageInfo.imageView = textureImageView;
    imageInfo.sampler = vulkanBE.textureSampler;

    //descriptor write info
    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(vulkanBE.mainDevices.device, 1, &descriptorWrite, 0, nullptr);

    vulkanBE.descriptors.samplerDescriptorSet[vulkanBE.descriptors.num_texture_descriptor_set] = descriptorSet;
    vulkanBE.descriptors.num_texture_descriptor_set++;

    return vulkanBE.descriptors.num_texture_descriptor_set-1;
}

static VkCommandBuffer begin_ot_cmd_submit(VkDevice* device, VkCommandPool* commandPool)
{
    //commandBuffer to hold transfer commands
    VkCommandBuffer commandBuffer;

    //command buffer details
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = *commandPool;
    allocInfo.commandBufferCount = 1;

    //Allocate command buffer from pool;
    vkAllocateCommandBuffers(*device, &allocInfo, &commandBuffer);

    //information to begin the commandbuffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //we're only using the command buffer once.

    //begin recording command for transfer
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

static void end_ot_cmd_submit(VkDevice* device, VkCommandPool* commandPool, VkQueue* queue, VkCommandBuffer* commandBuffer)
{
    vkEndCommandBuffer(*commandBuffer); //end command

    //queue submission
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffer;

    vkQueueSubmit(vulkanBE.transferQueue, 1, &submitInfo, VK_NULL_HANDLE);

    vkQueueWaitIdle(vulkanBE.transferQueue);

    vkFreeCommandBuffers(vulkanBE.mainDevices.device, vulkanBE.pools.transferCommandPool, 1, commandBuffer);
}

static void transitionImageLayout(VkImage image, VkImageLayout currLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = begin_ot_cmd_submit(&vulkanBE.mainDevices.device, &vulkanBE.pools.transferCommandPool);

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.oldLayout = currLayout;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                      .baseMipLevel = 0,
                                      .levelCount = 1,
                                      .baseArrayLayer = 0,
                                      .layerCount = 1};

    VkPipelineStageFlags srcStage = {};
    VkPipelineStageFlags dstStage = {};

    if(currLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) //if transitioning from new image to image ready to receive data...
    {
        imageMemoryBarrier.srcAccessMask = 0; //starting from any point in time (ie. 0). we want the transfer to happen before the dstAccessMask (ie VK_ACCESS_TRANSFER_WRITE_BIT).
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (currLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; //starting from any point in time (ie. 0). we want the transfer to happen before the dstAccessMask (ie VK_ACCESS_TRANSFER_WRITE_BIT).
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; //ready to be ready by shader before we get to fragment shader
    }

    vkCmdPipelineBarrier(commandBuffer,
                         srcStage,dstStage,                          //pipeline stages :match to src and dst access mask
                         0,                                     //dependency flags
                         0, nullptr,            //memory barrier count + data
                         0, nullptr,    //buffer memory count + data
                         1, &imageMemoryBarrier);

    end_ot_cmd_submit(&vulkanBE.mainDevices.device, &vulkanBE.pools.transferCommandPool, &vulkanBE.transferQueue, &commandBuffer);
}

static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
{
    VkCommandBuffer transferCommandBuffer = begin_ot_cmd_submit(&vulkanBE.mainDevices.device, &vulkanBE.pools.transferCommandPool);

    //copy region for vkCmdCopyBuffer
    VkBufferCopy bufferCopyRegion = {};
    bufferCopyRegion.srcOffset = 0; //we want to copy from the start and to the start
    bufferCopyRegion.dstOffset = 0;
    bufferCopyRegion.size = bufferSize;

    vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopyRegion); //command to copy out a src buffer to dst buffer

    end_ot_cmd_submit(&vulkanBE.mainDevices.device, &vulkanBE.pools.transferCommandPool, &vulkanBE.transferQueue, &transferCommandBuffer);
}

static void copyImageBuffer(VkBuffer srcBuffer, VkImage dstBuffer, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = begin_ot_cmd_submit(&vulkanBE.mainDevices.device, &vulkanBE.pools.transferCommandPool);

    //copy region for vkCmdCopyBuffer
    VkBufferImageCopy bufferImageRegion = {};
    bufferImageRegion.bufferOffset = 0; //offset into data
    bufferImageRegion.bufferRowLength = 0; //row length of data to calculate data spacing
    bufferImageRegion.bufferImageHeight = 0; //image height to calculate data spacing. this means that all the data is tightly packed
    bufferImageRegion.imageSubresource = {  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                            .mipLevel = 0,
                                            .baseArrayLayer = 0, //if we have an array of texture images;
                                            .layerCount = 1};
    bufferImageRegion.imageOffset = {0,0,0}; //the part of the image we want to copy starts at 0,0,0;
    bufferImageRegion.imageExtent = {width, height, 1}; //size of region to copy as x,y,z values

    vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageRegion); //command to copy out a src buffer to dst image
    //the destination image is going to have a transfer_dst_optimal layout

    end_ot_cmd_submit(&vulkanBE.mainDevices.device, &vulkanBE.pools.transferCommandPool, &vulkanBE.transferQueue, &commandBuffer);
}

static stbi_uc* loadTextureFile(char* filename, int* width, int* height, VkDeviceSize* imageSize)
{
    //number of channels image uses
    int channels;

    //load pixel data for image
    char fileLoc[80] = {};
    strcat(strcat(fileLoc, "../assets/textures/"), filename);
    stbi_uc * image = stbi_load(fileLoc, width, height, &channels, STBI_rgb_alpha);

    if (!image)
    {
        fprintf(stderr, "failed to load texture at location ");
        fprintf(stderr, fileLoc);
        fprintf(stderr, "\n");
        exit(1);
    }

    //calculate image size
    *imageSize = *width * *height * 4;

    return image;
}

static int create_texture_image(char* fileName)
{
    //load in the image file
    int width, height;
    VkDeviceSize imageSize;
    stbi_uc* imageData = loadTextureFile(fileName, &width, &height, &imageSize);

    //create staging buffer to hold loaded data, ready to copy to device
    VkBuffer imageStagingBuffer;
    VkDeviceMemory imageStagingBufferMemory;

    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &imageStagingBuffer, &imageStagingBufferMemory);

    void* data;
    vkMapMemory(vulkanBE.mainDevices.device, imageStagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, imageData, (size_t)imageSize);
    vkUnmapMemory(vulkanBE.mainDevices.device, imageStagingBufferMemory);

    stbi_image_free(imageData);

    //TODO: for now a DeviceMemory for each texture is fine but for optimal performance, use one DeviceMemory and use multiple VkImage with offsets

    //create Image to hold texture
    VkImage texImage;
    VkDeviceMemory texImageMemory;

    texImage = createImage(width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &texImageMemory);

    //transitioning to allow for transfer
    transitionImageLayout(texImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    //copy the data to image
    copyImageBuffer(imageStagingBuffer, texImage, width, height);

    //transitioning to allow for fragment shader read operation
    transitionImageLayout(texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    //Add texture data to vector for reference
    allTextures[num_textures].textureImage = texImage;
    allTextures[num_textures].textureImageMemory = texImageMemory;
    num_textures++;

    vkDestroyBuffer(vulkanBE.mainDevices.device, imageStagingBuffer, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, imageStagingBufferMemory, nullptr);

    return num_textures-1;
}

static void destroyUniformBuffers(VkBuffer* buffer, VkDeviceMemory* bufferMemory)
{
    vkDestroyBuffer(vulkanBE.mainDevices.device, *buffer, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, *bufferMemory, nullptr);
}

static void cleanUpBackend(struct Ve_backend* vulkanBackend)
{
    vkDeviceWaitIdle(vulkanBE.mainDevices.device);

    vkDestroySampler(vulkanBE.mainDevices.device, vulkanBE.textureSampler, nullptr);
    vkDestroyDescriptorPool(vulkanBE.mainDevices.device, vulkanBE.descriptors.samplerDescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(vulkanBE.mainDevices.device, vulkanBE.descriptors.samplerSetLayout, nullptr);

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for(int i = 0; i < num_textures; i++)
    {
        vkDestroyImageView(vulkanBE.mainDevices.device, allTextures[i].textureImageView, nullptr);
        vkDestroyImage(vulkanBE.mainDevices.device, allTextures[i].textureImage, nullptr);
        vkFreeMemory(vulkanBE.mainDevices.device, allTextures[i].textureImageMemory, nullptr);
    }

    vkDestroyImageView(vulkanBE.mainDevices.device, vulkanBE.depthBufferImages.imageView, nullptr);
    vkDestroyImage(vulkanBE.mainDevices.device, vulkanBE.depthBufferImages.image, nullptr);
    vkFreeMemory(vulkanBE.mainDevices.device, vulkanBE.depthBufferImages.imageMemory, nullptr);

    vkDestroyDescriptorPool(vulkanBE.mainDevices.device, vulkanBE.descriptors.descriptorPool, nullptr);
    vkDestroyDescriptorPool(vulkanBE.mainDevices.device, vulkanBE.descriptors.imGuiDescriptorPool, nullptr);

    for(size_t i = 0; i < MAX_IMAGE; i++)
    {
        destroyUniformBuffers(&vulkanBE.descriptors.uniformBuffer[i], &vulkanBE.descriptors.uniformBufferMemory[i]);
        destroyUniformBuffers(&vulkanBE.descriptors.light_UB[i], &vulkanBE.descriptors.light_UBM[i]);
    }

    vkDestroyDescriptorSetLayout(vulkanBE.mainDevices.device, vulkanBE.descriptors.descriptorSetLayout, nullptr);

    for(size_t i = 0; i< MAX_FRAME_DRAWS; i++)
    {
        vkDestroySemaphore(vulkanBE.mainDevices.device, vulkanBE.sync.renderFinished[i], nullptr);
        vkDestroySemaphore(vulkanBE.mainDevices.device, vulkanBE.sync.imageAvailable[i], nullptr);
        vkDestroyFence(vulkanBE.mainDevices.device, vulkanBE.sync.drawFences[i], nullptr);
    }

    vkDestroyCommandPool(vulkanBE.mainDevices.device, vulkanBE.pools.graphicsCommandPool, nullptr);

    for(int i = 0; i<vulkanBE.num_swapchain_images; i++)
    {
        vkDestroyFramebuffer(vulkanBE.mainDevices.device, vulkanBE.swapChainFrameBuffers[i], nullptr);
    }

    for(int i = 0; i<vulkanBE.num_pipelines; i++)
    {
        vkDestroyPipeline(vulkanBE.mainDevices.device, vulkanBE.graphicsPipeline[i], nullptr);
        vkDestroyPipelineLayout(vulkanBE.mainDevices.device, vulkanBE.pipelineLayout[i], nullptr);
    }

    vkDestroyRenderPass(vulkanBE.mainDevices.device,vulkanBE.renderPass,nullptr);

    for(int i = 0; i<vulkanBE.num_swapchain_images; i++)
    {
        vkDestroyImageView(vulkanBE.mainDevices.device, vulkanBE.swapChainImages[i].imageView, nullptr);
    }
    vkDestroySwapchainKHR(vulkanBE.mainDevices.device,vulkanBE.swapChain,nullptr);
    vkDestroySurfaceKHR(vulkanBE.instance, vulkanBE.surface, nullptr);
    vkDestroyDevice(vulkanBE.mainDevices.device, nullptr);
    if (enableValidationLayers) {DestroyDebugUtilsMessengerEXT(vulkanBE.instance, debugMessenger, nullptr);}
    vkDestroyInstance(vulkanBE.instance, nullptr);
    cleanUpVulkanBE();
}

static void initBackend(){

    initVulkanBE(vulkanBE);

    //no try catch here so be careful
    initWindow((char*)"Hello",1200,900);
    createInstance();
    setupDebugMessenger(vulkanBE.instance);
    createSurface();
    getPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createRenderPass();
    createPushConstantRange();
    createDescriptorSetLayout();
    createGraphicsPipeline(VK_POLYGON_MODE_FILL);
    createGraphicsPipeline(VK_POLYGON_MODE_LINE);
    createDepthBufferImage();
    createFrameBuffers();
    createCommandPool();
    createCommandBuffers();
    createTextureSampler();
    createDescriptorPool();
    createDescriptorSet();
    createSynchronization();
}