cmake_minimum_required(VERSION 3.20)
project(SimpleEngine)

set(CMAKE_CXX_STANDARD 11)

include_directories(C:/GLFW/include)
include_directories(C:/VulkanSDK/1.2.189.2/Include)
include_directories(C:/GLM/glm)
include_directories(${PROJECT_SOURCE_DIR}/external/imgui)

add_executable(SimpleEngine source/main.cpp source/window/ve_window.cpp source/window/ve_window.h source/vulkan-backend/ve_backend.cpp source/vulkan-backend/ve_backend.h source/vulkan-backend/ve_validation_layer.cpp source/vulkan-backend/ve_validation_layer.h source/utilities/utilities.h source/renderer/ve_renderer.cpp source/renderer/ve_renderer.h source/utilities/shapes.h source/renderer/kb_input.cpp source/renderer/kb_input.h)

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

target_link_libraries(SimpleEngine "C:/GLFW/lib-mingw-w64/libglfw3.a")
target_link_libraries(SimpleEngine "C:/VulkanSDK/1.2.189.2/Lib/vulkan-1.lib")
target_link_libraries(SimpleEngine imm32.lib)
