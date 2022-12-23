cmake_minimum_required(VERSION 3.20)
project(SimpleEngine)

set(CMAKE_CXX_STANDARD 17)

#Find the library needed for the project##############
find_library(GLFW_lib glfw3 NAMES glfw3 glfw3dll PATHS "${PROJECT_SOURCE_DIR}/external/GLFW/lib-mingw-w64")
if(${GLFW_lib} STREQUAL GLFW_lib-NOTFOUND)
    message(FATAL_ERROR "glfw library has not been found")
else()
    message(STATUS "glfw found! as ${GLFW_lib}")
endif()

find_library(Vulkan_lib vulkan NAMES vulkan-1 PATHS "C:/VulkanSDK/1.3.216.0/Lib")
if(${Vulkan_lib} STREQUAL Vulkan_lib-NOTFOUND)
    message(FATAL_ERROR "Vulkan library has not been found")
else()
    message(STATUS "vulkan found! as ${Vulkan_lib}")
endif()
######################################################

add_executable(SimpleEngine
        source/main.cpp
        source/window/ve_window.cpp
        source/window/ve_window.h
        source/vulkan-backend/ve_backend.cpp
        source/vulkan-backend/ve_backend.h
        source/vulkan-backend/ve_validation_layer.cpp
        source/vulkan-backend/ve_validation_layer.h
        source/utilities/utilities.h
        source/renderer/ve_renderer.cpp
        source/renderer/ve_renderer.h
        source/utilities/shapes.h
        source/utilities/kb_input.cpp
        source/utilities/kb_input.h)

target_sources(SimpleEngine PUBLIC
        "external/imgui/imgui.h"
        "external/imgui/imgui.cpp"

        "external/imgui/imgui_demo.cpp"
        "external/imgui/imgui_draw.cpp"
        "external/imgui/imgui_widgets.cpp"
        "external/imgui/imgui_tables.cpp"

        "external/imgui/backends/imgui_impl_vulkan.cpp"
        "external/imgui/backends/imgui_impl_glfw.cpp"

        "external/stb_image.h")

#add GLFW and GLM include directory
target_include_directories(SimpleEngine PUBLIC
        "${PROJECT_SOURCE_DIR}/external/GLFW/include"
        "${PROJECT_SOURCE_DIR}/external/GLM/glm"
        "C:/VulkanSDK/1.3.216.0/Include"
        "${PROJECT_SOURCE_DIR}/external/imgui")

target_link_libraries(SimpleEngine ${GLFW_lib})
target_link_libraries(SimpleEngine ${Vulkan_lib})
target_link_libraries(SimpleEngine imm32.lib)
