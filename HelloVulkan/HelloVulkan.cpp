#include "HelloVulkan.h"
#include <fstream>

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

HelloVulkan::~HelloVulkan()
{
    for (auto &&i : this->swapChainImageViews)
    {
        vkDestroyImageView(this->device, i, nullptr);
    }
    
    vkDestroySwapchainKHR(this->device, this->swapChain, nullptr);
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
        VkExtent2D actualExtent = { WIDTH, HEIGHT };
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
        throw std::runtime_error(path + " could not be opened");
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
        throw std::runtime_error("Shader Module could not be created");
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
        throw std::runtime_error("Could not create vulkan instance");            
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
        throw std::runtime_error("Could not create vulkan instance");
    }
}
    
void HelloVulkan::createSurface()
{
    if(VK_SUCCESS != glfwCreateWindowSurface(this->instance, this->window, nullptr, &this->surface))
    {
        throw std::runtime_error("could not create window surface");
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
            throw std::runtime_error("Cound not setup the debug messenger.");
        }
    }
}

void HelloVulkan::pickPhysicalDevice()
{
    uint32_t devicesCount;
    vkEnumeratePhysicalDevices(this->instance, &devicesCount, nullptr);
    if(devicesCount == 0)
    {
        throw std::runtime_error("No GPUs with vulkan support found on the system");
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
        throw std::runtime_error("failed to find a suitable GPU!");
    }
    if (physicalDevice == VK_NULL_HANDLE) 
    {
        throw std::runtime_error("failed to find a suitable GPU!");
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
        throw std::runtime_error("failed to create a logical device");
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
        throw std::runtime_error("Could not create swap chain");
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

    VkPipelineShaderStageCreateInfo crateInfos[] = {vertShaderStageCreateInfo, fragmentShaderCreateInfo};

    vkDestroyShaderModule(this->device, vertexShaderModule, nullptr);
    vkDestroyShaderModule(this->device, fragmentShaderModule, nullptr);
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
    createGraphicsPipeline();
    return true;
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
            throw std::runtime_error("Could not create swap chain imaga views");
        }
    }
}

void HelloVulkan::loop()
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }
}