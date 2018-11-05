cmake_minimum_required(VERSION 3.10)

project(vulkan_tutorial)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    main.cpp
)

add_executable(vulkan_tutorial ${SOURCES})

find_package(Vulkan REQUIRED)
find_package(glfw3 3.2 REQUIRED)

target_link_libraries(vulkan_tutorial Vulkan::Vulkan glfw)
