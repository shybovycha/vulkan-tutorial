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

class Application {
protected:
    GLFWwindow *window;

public:
    void run() {
        initVulkan();
        createWindow();

        mainLoop();

        cleanup();
    }

private:
    void initVulkan() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::cout << extensionCount << " extensions supported" << std::endl;

        glm::mat4 matrix;
        glm::vec4 vec;
        auto test = matrix * vec;

        std::cout << "matrix x vector = " << glm::to_string(test) << std::endl;
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
