CFLAGS = -std=c++17 
LDFLAGS = `pkg-config --static --libs glfw3` -lvulkan
VULKAN_SDK_PATH = /home/SDKs/Vulkan/1.1.126.0/x86_64

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp HelloVulkan.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/vulkan/explicit_layer.d
	./VulkanTest

clean:
	rm -f VulkanTest