#include <vulkan/vulkan.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

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
    SDL_Window *window;
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
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledLayerCount = 0;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }

    void checkVkExtensions() {
        uint32_t extensionCount = 0;
        // vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);

        // std::vector<VkExtensionProperties> extensions(extensionCount);

        // vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::vector<const char*> extensions(extensionCount);

        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

        std::cout << "Available " << extensionCount << " Vulkan extensions:" << std::endl;

        for (const auto& extension : extensions) {
            // std::cout << "\t" << extension.extensionName << std::endl;
            std::cout << "\t" << extension << std::endl;
        }
    }

    void createWindow() {
        SDL_Surface* screenSurface = nullptr;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialize. SDL_Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("SDL could not initialize");
        }

        window = SDL_CreateWindow("Vulkan Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (window == nullptr) {
            std::cout << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
            throw std::runtime_error("Can not open SDL window");
        }
    }

    void mainLoop() {
        SDL_Event e;
        bool isRunning = true;

        while (isRunning) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    isRunning = false;
                }
            }
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);

        SDL_DestroyWindow(window);

        SDL_Quit();
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
