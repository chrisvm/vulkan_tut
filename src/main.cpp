
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include "vk_deleter.h"

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApplication {
public:
    void run()
	{
		initWindow();
        initVulkan();
        mainLoop();
    }

private:
	GLFWwindow* window;
	void initWindow()
	{
		// init without opengl, will use vulkan
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	VDeleter<VkInstance> instance {vkDestroyInstance};
	void createInstance()
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions =
			glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		for (unsigned int index = 0; index < glfwExtensionCount; ++index) {
			std::cout << "GLFW Needed Exception: " << glfwExtensions[index] << std::endl;
		}
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		// exit if unable to create vk instance
		if (vkCreateInstance(&createInfo, nullptr, instance.replace()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vk instance");
		}

		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr,
			&extensionCount, extensions.data());
		if (!validateGLFWRequiredExtensions(glfwExtensionCount, glfwExtensions, extensions)) {
			throw std::runtime_error("some extensions required by GLFW are missing");
		}
		std::cout << "Extension Count: " << extensionCount << std::endl;
		for (const auto& extension : extensions) {
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	}

	bool validateGLFWRequiredExtensions(uint32_t count,
		const char** glfwExtensions,
		std::vector<VkExtensionProperties> extensions)
	{
		unsigned short found = 0;
		for (unsigned int index = 0; index < count; ++index) {
			for (const auto& ext : extensions) {
				if (!strcmp(glfwExtensions[index], ext.extensionName)) {
					std::cout << "Found " << ext.extensionName << std::endl;
					found += 1;
				}
			}
		}
		return count >= found;
	}

	void initVulkan()
	{
		createInstance();
    }

    void mainLoop()
	{
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
