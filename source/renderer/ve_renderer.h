//
// Created by hlahm on 2021-10-21.
//

#ifndef VULKANENGINE2_VE_RENDERER_H
#define VULKANENGINE2_VE_RENDERER_H

//imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "../vulkan-backend/ve_backend.cpp"

static float frameTime = 0;

struct Ve_renderer{
    void (*run)();
};

struct Camera{
    glm::mat4 projection;
    glm::mat4 view;
};

struct Lights{
    glm::vec4 light1;
    glm::vec4 light2;
};

struct Gui{
    ImDrawData* draw_data;
};

#endif //VULKANENGINE2_VE_RENDERER_H
