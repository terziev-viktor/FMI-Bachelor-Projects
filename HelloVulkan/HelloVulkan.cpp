#include "HelloVulkan.h"

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

uint32_t HelloVulkan::rateGPU(VkPhysicalDevice gpu)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(gpu, &properties);
    std::cout << "\tgot gpu properties\n";
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(gpu, &features);
    std::cout << "\tgot gpu features\n";

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

QueueFamilyIndices HelloVulkan::findQueueFamilies() 
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    
    std::cout << "found queue families!\n";
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    uint32_t j = 0;
    for (auto &&i : queueFamilies)
    {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, j, surface, &presentSupport);

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

bool HelloVulkan::isDeviceSuitable(const VkPhysicalDeviceProperties & properties, const VkPhysicalDeviceFeatures & features, const VkPhysicalDevice & gpu)
{   
    return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;
}

bool HelloVulkan::checkValidationLayerSupport()
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
    std::cout << "\t" << devicesCount << " candidates\n";
    std::multimap<uint32_t, VkPhysicalDevice> candidates;
    std::vector<VkPhysicalDevice> devices(devicesCount);
    vkEnumeratePhysicalDevices(this->instance, &devicesCount, devices.data());
    std::cout << "\t" << " candidates enumerated\n";
    for (auto &&i : devices)
    {
        uint32_t score = rateGPU(i);
        std::cout << "\tdevice score=" << score << "\n";
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
    QueueFamilyIndices indices = findQueueFamilies();
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
    createInfo.enabledExtensionCount = 0;
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
        std::cout << "instanceCreated \n";
    setupDebugMessanger();
        std::cout << "debugMessanger created \n";
    createSurface();
        std::cout << "surface created \n";
    pickPhysicalDevice();
        std::cout << "picked a physical device\n";
    createLogicalDevice();
        std::cout << "created a logical device\n";
    return true;
}

void HelloVulkan::loop()
{
    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();
    }
}