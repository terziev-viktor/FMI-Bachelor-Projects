#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <string.h>
#include <map>
#include <set>


void populateDebugMessenerInfo(VkDebugUtilsMessengerCreateInfoEXT& info);

struct QueueFamilyIndices 
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() 
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

VkResult CreateDebugMessengerEXT(const VkInstance & instance, const
VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const
VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT*
pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
VkDebugUtilsMessengerEXT debugMessenger, const
VkAllocationCallbacks* pAllocator);

class HelloVulkan
{
public:
    HelloVulkan();

    ~HelloVulkan();

    inline operator bool()
    {
        return this->isInit;
    }

    inline void run()
    {
        loop();
    }

    const int WIDTH = 800;
    const int HEIGHT = 600;

    const std::vector<const char*> validationLayers = 
    {
        "VK_LAYER_KHRONOS_validation"
    };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    bool checkValidationLayerSupport();

    ///
    /// Debug callback function
    /// * msgSeverity is one of:
    ///   VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
    ///   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
    ///   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
    ///   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
    ///
    /// *  msgType is one of:
    ///    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT = Some event has hap-pened that is unrelated to the specification or performance
    ///    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT = Something hashappened that violates the specification or indicates a possible mistake
    ///    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT = Potential non-optimal use of Vulkan
    ///
    /// * pCallbackData contains details of the message, the most important being:
    ///   pMessage = The debug message as a null-terminated string
    ///   pObjects = Array of Vulkan object handles related to the message
    ///   objectCount: Number of objects in array
    /// 
    /// * pUserData parameter contains a pointer that was specified during the setup of the callback and allows you to pass your own data to it.
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT msgType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData);

private:
    bool isInit;
    GLFWwindow * window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;

    QueueFamilyIndices findQueueFamilies();

    bool isDeviceSuitable(const VkPhysicalDeviceProperties & properties, const VkPhysicalDeviceFeatures & features, const VkPhysicalDevice & gpu);
    
    uint32_t rateGPU(VkPhysicalDevice gpu);

    void createInstance();
    void createSurface();

    std::vector<const char*> getRequiredExtensions();

    void setupDebugMessanger();

    void pickPhysicalDevice();

    void createLogicalDevice();

    bool init();

    void loop();
};