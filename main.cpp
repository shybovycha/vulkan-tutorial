#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <memory>
#include <vector>

class Application {
protected:
    GLFWwindow *window;
    VkInstance instance;

public:
    void run() {
        initVulkan();

        createWindow();

        mainLoop();

        cleanup();
    }

private:
    void initVulkan() {
        createVkInstance();
        checkVkExtensions();
    }

    void createVkInstance() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Tutorial";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void checkVkExtensions() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << extensionCount << " Vulkan extensions supported" << std::endl;

        for (const auto& extension : extensions) {
            std::cout << " - " << extension.extensionName << std::endl;
        }
    }

    void createWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        cleanVulkan();
        cleanGlfw();
    }

    void cleanVulkan() {
        vkDestroyInstance(instance, nullptr);
    }

    void cleanGlfw() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main() {
    std::unique_ptr<Application> app = std::make_unique<Application>();

    try {
        app->run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
