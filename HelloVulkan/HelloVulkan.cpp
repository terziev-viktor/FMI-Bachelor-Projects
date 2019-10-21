#include "HelloVulkan.h"
#include <fstream>
#include <assert.h>
#include <chrono>
#include <thread>

void log(const char * msg)
{
    std::cout << msg << std::endl;
}

void populateDebugMessenerInfo(VkDebugUtilsMessengerCreateInfoEXT& info)
{
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    info.pfnUserCallback = HelloVulkan::debugCallback;
}

VkResult CreateDebugMessengerEXT(const VkInstance & instance, const
VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const
VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT*
pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else 
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
VkDebugUtilsMessengerEXT debugMessenger, const
VkAllocationCallbacks* pAllocator) 
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) 
    {
        func(instance, debugMessenger, pAllocator);
    }
}

HelloVulkan::HelloVulkan()
{
    isInit = this->init();
}

bool HelloVulkan::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if(window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr); NULL == window)
    {
        return false;
    }
    createInstance();
    setupDebugMessanger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffers();
    createSyncObjects();
    return true;
}

HelloVulkan::~HelloVulkan()
{   
    for (size_t i = 0; i < this->MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkDestroySemaphore(this->device, this->renderFinishedSemaphors[i], nullptr);
        vkDestroySemaphore(this->device, this->imageAvailableSemaphors[i], nullptr);
        vkDestroyFence(this->device, this->inFlightFences[i], nullptr);
    }

    cleanupSwapChain();
    
    vkDestroyCommandPool(this->device, this->commandPool, nullptr);

    vkDestroyDevice(this->device, nullptr);
    if(enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(this->instance, this->debugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
    vkDestroyInstance(this->instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

uint32_t HelloVulkan::rateGPU(VkPhysicalDevice gpu) const
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(gpu, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(gpu, &features);

    if (!isDeviceSuitable(properties, features, gpu))
    {
        return 0;
    }
    
    uint32_t score = 0;
    // Discrete GPUs have a significant performance advantage
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    
    // Maximum possible size of textures affects graphics quality
    score += properties.limits.maxImageDimension2D;

    return score;
}

QueueFamilyIndices HelloVulkan::findQueueFamilies(VkPhysicalDevice gpu) const
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueFamilies.data());

    uint32_t j = 0;
    for (auto &&i : queueFamilies)
    {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(gpu, j, surface, &presentSupport);

        if (i.queueCount > 0 && presentSupport)
        {
            indices.presentFamily = j;
        }
        
        if (i.queueCount > 0 && i.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = j;
        }

        if (indices.isComplete())
        {
            break;
        }

        ++j;
    }
    

    return indices;
}

SwapChainSupportDetails HelloVulkan::querySwapChainSupport(VkPhysicalDevice gpu) const
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &details.capabilities);
    uint32_t count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, nullptr);

    if (count != 0)
    {
        details.surfaceFormats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &count, details.surfaceFormats.data());
    }

    count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, this->surface, &count, nullptr);
    if(count != 0)
    {
        details.presentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, this->surface, &count, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR HelloVulkan::chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> & availableFormats) const
{
    for (const auto &i : availableFormats)
    {
        if (i.format == VK_FORMAT_B8G8R8_UNORM && i.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return i;
        }
    }

    return availableFormats[0]; // or choose with some custom logic
}

VkPresentModeKHR HelloVulkan::chooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> & availableModes) const
{
    auto best = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto &i : availableModes)
    {
        if (i == VK_PRESENT_MODE_MAILBOX_KHR) // MAILBOX is what we want but FIFO is guaranteed to be available
        {
            return i;
        }
        else if(i == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            best = i;
        }
        
    }
    
    return best;
}

VkExtent2D HelloVulkan::chooseSwapChainExtent2D(const VkSurfaceCapabilitiesKHR & capabilities) const
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(this->window, &width, &height);
        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

bool HelloVulkan::isDeviceSuitable(const VkPhysicalDeviceProperties & properties, const VkPhysicalDeviceFeatures & features, const VkPhysicalDevice & gpu) const
{   
    QueueFamilyIndices indices = findQueueFamilies(gpu);
    bool hasExtensions = checkDeviceExtensionSupport(gpu);
    bool swapChainAdequate = false;
    if (hasExtensions)
    {
        SwapChainSupportDetails swapChainDetails = querySwapChainSupport(gpu);
        swapChainAdequate = !swapChainDetails.presentModes.empty() && !swapChainDetails.surfaceFormats.empty();
        // add restrictions for the swapChainDetails.capabilities if you wish
    }
    return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
    && features.geometryShader 
    && hasExtensions 
    && indices.isComplete()
    && swapChainAdequate;
}

bool HelloVulkan::checkValidationLayerSupport() const
{
    uint32_t layersCount;
    vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

    std::vector<VkLayerProperties> layerProp(layersCount);
    vkEnumerateInstanceLayerProperties(&layersCount, layerProp.data());
    
    for(const char * i : validationLayers)
    {
        bool found = false;
        for (auto &&j : layerProp)
        {
            if (0 == strcmp(j.layerName, i))
            {
                found = true;
                break;
            }
            
        }
        if (!found)
        {
            std::cout << "validation layer " << i << " not available\n";
            return false;
        }
    }
    return true;
}

std::vector<char> HelloVulkan::readFile(const std::string & path)
{
    std::ifstream ifs(path, std::ios::ate | std::ios::binary);
    if(!ifs.is_open())
    {
        assert(!"Could not open file");
    }
    size_t fileSize = static_cast<size_t>(ifs.tellg());
    std::vector<char> buffer(fileSize);
    ifs.seekg(0);
    ifs.read(buffer.data(), fileSize);
    ifs.close();

    return buffer; // ofc move c-tor will be used here so np
}

VkShaderModule HelloVulkan::createShaderModule(const std::vector<char> & shaderCode)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
    VkShaderModule shaderModule;
    if(VK_SUCCESS != vkCreateShaderModule(this->device, &createInfo, nullptr, &shaderModule))
    {
        assert(!"Shader Module could not be created");
    }
    return shaderModule;
}

bool HelloVulkan::checkDeviceExtensionSupport(VkPhysicalDevice gpu) const
{
    uint32_t count = 0;
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, nullptr);

    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateDeviceExtensionProperties(gpu, nullptr, &count, extensions.data()); // get all available extensions

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); // get all required extensions

    for (auto &&i : extensions)
    {
        requiredExtensions.erase(i.extensionName); // 'check' each required extension
    }

    return requiredExtensions.empty(); // all required extensions should be checked
}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloVulkan::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT msgType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData)
{
    std::cerr << "validation layer says: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

void HelloVulkan::createInstance()
{
    VkApplicationInfo info;
    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName = "Hello, Vulkan!";
    info.applicationVersion = VK_MAKE_VERSION(1, 0, 1);
    info.pEngineName = "No Engine";
    info.engineVersion = VK_MAKE_VERSION(1, 0, 1);
    info.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &info;
    
    if(this->enableValidationLayers && !this->checkValidationLayerSupport())
    {
        assert(!"Could not create vulkan instance");            
    }
    std::vector<const char*> glfwExtensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = glfwExtensions.size();
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();
    VkDebugUtilsMessengerCreateInfoEXT debugInfo;
    if(this->enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessenerInfo(debugInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    VkResult result = vkCreateInstance(&createInfo, nullptr, &this->instance);
    if (VK_SUCCESS != result)
    {
        assert(!"Could not create vulkan instance");
    }
}
    
void HelloVulkan::createSurface()
{
    if(VK_SUCCESS != glfwCreateWindowSurface(this->instance, this->window, nullptr, &this->surface))
    {
        assert(!"could not create window surface");
    }
}

std::vector<const char*> HelloVulkan::getRequiredExtensions()
{
    uint32_t glfwExtensionsCount = 0;
    const char ** glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char*> extensions(glfwExtensionNames, glfwExtensionNames + glfwExtensionsCount);
    if (enableValidationLayers) 
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

void HelloVulkan::setupDebugMessanger()
{
    if(enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT info = {};
        populateDebugMessenerInfo(info);
        info.pUserData = this; // optional
        if(VK_SUCCESS != CreateDebugMessengerEXT(this->instance, &info, nullptr, &this->debugMessenger))
        {
            assert(!"Cound not setup the debug messenger.");
        }
    }
}

void HelloVulkan::pickPhysicalDevice()
{
    uint32_t devicesCount;
    vkEnumeratePhysicalDevices(this->instance, &devicesCount, nullptr);
    if(devicesCount == 0)
    {
        assert(!"No GPUs with vulkan support found on the system");
    }
    std::multimap<uint32_t, VkPhysicalDevice> candidates;
    std::vector<VkPhysicalDevice> devices(devicesCount);
    vkEnumeratePhysicalDevices(this->instance, &devicesCount, devices.data());
   
    for (auto &&i : devices)
    {
        uint32_t score = rateGPU(i);
        
        candidates.insert(std::make_pair(score, i));
    }
    if (candidates.rbegin()->first > 0) 
    {
        physicalDevice = candidates.rbegin()->second;
    }
    else
    {
        assert(!"failed to find a suitable GPU!");
    }
    if (physicalDevice == VK_NULL_HANDLE) 
    {
        assert(!"failed to find a suitable GPU!");
    }
}

void HelloVulkan::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(this->physicalDevice);
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (auto &&queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority; // required even if we have only one queue
    VkPhysicalDeviceFeatures deviceFeatures = {};
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t> (queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(this->deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = this->deviceExtensions.data();
    if (enableValidationLayers) 
    {
        createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } 
    else
    {
        createInfo.enabledLayerCount = 0;
    }
    if(VK_SUCCESS != vkCreateDevice(this->physicalDevice, &createInfo, nullptr, &this->device))
    {
        assert(!"failed to create a logical device");
    }
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void HelloVulkan::createSwapChain()
{
    SwapChainSupportDetails details = querySwapChainSupport(this->physicalDevice);

    VkSurfaceFormatKHR format = chooseSwapChainSurfaceFormat(details.surfaceFormats);
    VkPresentModeKHR mode = chooseSwapChainPresentMode(details.presentModes);
    VkExtent2D extent = chooseSwapChainExtent2D(details.capabilities);

    uint32_t imageCount = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) 
    {
        imageCount = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = this->surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    QueueFamilyIndices indices = findQueueFamilies(this->physicalDevice);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value()};
    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // optional
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.presentMode = mode;
    createInfo.clipped = VK_TRUE;
    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (VK_SUCCESS != vkCreateSwapchainKHR(this->device, &createInfo, nullptr, &this->swapChain))
    {
        assert(!"Could not create swap chain");
    }

    uint32_t count = 0;
    vkGetSwapchainImagesKHR(this->device, this->swapChain, &count, nullptr);
    if (count != 0)
    {
        this->swapChainImages.resize(count);
        vkGetSwapchainImagesKHR(this->device, this->swapChain, &count, this->swapChainImages.data());
    }
    this->swapChainExtent = extent;
    this->swapChainImageFormat = format.format;
}

void HelloVulkan::cleanupSwapChain()
{
    for (auto &&framebuffer : this->swapChainFramebuffers)
    {
        vkDestroyFramebuffer(this->device, framebuffer, nullptr);
    }
    vkFreeCommandBuffers(this->device, this->commandPool, this->commandBuffers.size(), this->commandBuffers.data());
    vkDestroyPipeline(this->device, this->graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(this->device, this->pipelineLayout, nullptr);
    vkDestroyRenderPass(this->device, this->renderPass, nullptr);
    for (auto &&i : this->swapChainImageViews)
    {
        vkDestroyImageView(this->device, i, nullptr);
    }
    
    vkDestroySwapchainKHR(this->device, this->swapChain, nullptr);

}

void HelloVulkan::recreateSwapChain()
{
    vkDeviceWaitIdle(this->device);

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandBuffers();
}

void HelloVulkan::createRenderPass()
{
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

    VkSubpassDependency subpassDep = {};
    subpassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDep.dstSubpass = 0;
    subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDep.srcAccessMask = 0;
    subpassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &subpassDep;

    if(VK_SUCCESS != vkCreateRenderPass(this->device, &renderPassInfo, nullptr, &this->renderPass)) 
    {
        assert(!"Could not create Render Pass");
    }

    
}

void HelloVulkan::createGraphicsPipeline()
{
    std::vector<char> vertexShader = readFile("shaders/vert.spv");
    std::vector<char> fragmentShader = readFile("shaders/frag.spv");

    VkShaderModule vertexShaderModule = this->createShaderModule(vertexShader);
    VkShaderModule fragmentShaderModule = this->createShaderModule(fragmentShader);

    VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};
    vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageCreateInfo.module = vertexShaderModule;
    vertShaderStageCreateInfo.pName = "main";
    
    VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderCreateInfo.module = fragmentShaderModule;
    fragmentShaderCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragmentShaderCreateInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
    vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
    vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
    inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewPort = {};
    viewPort.x = 0.0f;
    viewPort.y = 0.0f;
    viewPort.width = swapChainExtent.width;
    viewPort.height = swapChainExtent.height;
    viewPort.minDepth = 0.0f;
    viewPort.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    
    VkPipelineViewportStateCreateInfo viewPortCreateInfo = {};
    viewPortCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewPortCreateInfo.pScissors = &scissor;
    viewPortCreateInfo.scissorCount = 1;
    viewPortCreateInfo.pViewports = &viewPort;
    viewPortCreateInfo.viewportCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    uint32_t dynamicStatesCount = 2;
    VkDynamicState dynamicStates[dynamicStatesCount] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = dynamicStatesCount;
    dynamicStateCreateInfo.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
    if (VK_SUCCESS != vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout))
    {
        assert(!"Could not create pipeline layout");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
    pipelineInfo.pViewportState = &viewPortCreateInfo;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    if(VK_SUCCESS != vkCreateGraphicsPipelines(this->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->graphicsPipeline))
    {
        assert(!"Could not create graphics pipeline ;(");
    }
    vkDestroyShaderModule(this->device, vertexShaderModule, nullptr);
    vkDestroyShaderModule(this->device, fragmentShaderModule, nullptr);
}

void HelloVulkan::createFramebuffers() 
{
    size_t sz = this->swapChainImageViews.size();
    VkFramebufferCreateInfo fbinfo = {};
    fbinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbinfo.renderPass = renderPass;
    fbinfo.attachmentCount = 1;
    fbinfo.width = swapChainExtent.width;
    fbinfo.height = swapChainExtent.height;
    fbinfo.layers = 1;

    this->swapChainFramebuffers.resize(sz);
    for (size_t i = 0; i < sz; ++i)
    {
        VkImageView attachments[] = { swapChainImageViews[i] };
        fbinfo.pAttachments = attachments;

        if (VK_SUCCESS != vkCreateFramebuffer(device, &fbinfo, nullptr, &swapChainFramebuffers[i])) 
        {
            assert(!"Failed to create framebuffer");
        }
    }
    
}

void HelloVulkan::createCommandPool()
{
    QueueFamilyIndices indices = findQueueFamilies(this->physicalDevice);
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.queueFamilyIndex = indices.graphicsFamily.value();
    info.flags = 0;

    if (VK_SUCCESS != vkCreateCommandPool(device, &info, nullptr, &commandPool)) 
    {
        assert(!"failed to create command pool!");
    }
}

void HelloVulkan::createCommandBuffers()
{
    size_t sz = this->swapChainFramebuffers.size();

    this->commandBuffers.resize(sz);
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if(VK_SUCCESS != vkAllocateCommandBuffers(this->device, &allocInfo, this->commandBuffers.data()))
    {
        assert(!"failed to create command buffers");
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;
    for (size_t i = 0 ; i < sz; ++i)
    {
        if(VK_SUCCESS != vkBeginCommandBuffer(this->commandBuffers[i], &beginInfo))
        {
            assert(!"Failed to begin recording command buffer");
        }
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = this->renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = this->swapChainExtent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
        vkCmdEndRenderPass(commandBuffers[i]);
        if (VK_SUCCESS != vkEndCommandBuffer(commandBuffers[i])) 
        {
            assert(!"failed to record command buffer!");
        }
    }
    
    
}

void HelloVulkan::createSyncObjects()
{
    this->imageAvailableSemaphors.resize(this->MAX_FRAMES_IN_FLIGHT);
    this->renderFinishedSemaphors.resize(this->MAX_FRAMES_IN_FLIGHT);
    this->inFlightFences.resize(this->MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoresInfo = {};
    semaphoresInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < this->MAX_FRAMES_IN_FLIGHT; ++i)
    {
        if(VK_SUCCESS != vkCreateSemaphore(this->device, &semaphoresInfo, nullptr, &this->renderFinishedSemaphors[i]) ||
        VK_SUCCESS != vkCreateSemaphore(this->device, &semaphoresInfo, nullptr, &this->imageAvailableSemaphors[i]) ||
        VK_SUCCESS != vkCreateFence(this->device, &fenceInfo, nullptr, &this->inFlightFences[i]))
        {
            assert(!"Could not create semaphors");
        }
    }
}

void HelloVulkan::createImageViews()
{
    size_t swapChainImagesSize = this->swapChainImages.size();
    this->swapChainImageViews.resize(swapChainImagesSize);
    
    for (size_t i = 0; i < swapChainImagesSize; ++i)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (VK_SUCCESS != vkCreateImageView(this->device, &createInfo, nullptr, &this->swapChainImageViews[i]))
        {
            assert(!"Could not create swap chain imaga views");
        }
    }
}

void HelloVulkan::drawFrame()
{
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFences[currentFrame]);
    uint32_t imageIndex;
    VkResult r = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphors[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if(VK_ERROR_OUT_OF_DATE_KHR == r) 
    {
        recreateSwapChain();
    }
    else if(VK_SUCCESS != r && VK_SUBOPTIMAL_KHR != r)
    {
        assert(!"Could not acquire next image");
    }
    else
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                                        
        VkSemaphore waitSemaphores[] = { imageAvailableSemaphors[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphors[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) 
        {
            assert(!"failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(presentQueue, &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}

void HelloVulkan::loop()
{
    uint32_t t = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(this->device);
}