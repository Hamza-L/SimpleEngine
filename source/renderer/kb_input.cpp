//
// Created by hlahm on 2021-11-07.
//

#include "kb_input.h"

void static key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    {

        if (key == GLFW_KEY_UP && action == GLFW_PRESS){
            UP_PRESS = true;
        } else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
            UP_PRESS = false;
        }

        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
            DOWN = true;
        } else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
            DOWN = false;
        }

        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
            RIGHT = true;
        } else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
            RIGHT = false;
        }

        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
            LEFT = true;
        } else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
            LEFT = false;
        }

        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
            SHIFT = true;
        } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
            SHIFT = false;
        }

        if (key == GLFW_KEY_LEFT_SUPER && action == GLFW_PRESS){
            COM = true;
        } else if (key == GLFW_KEY_LEFT_SUPER && action == GLFW_RELEASE) {
            COM = false;
        }
    }
}

void static mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)){
        MPRESS_L = true;
    } else {
        MPRESS_L = false;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        MFLAG_L = true;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)){
        MPRESS_R = true;
    } else {
        MPRESS_R = false;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        MFLAG_R = true;
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)){
        MPRESS_M = true;
    } else {
        MPRESS_M = false;
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE){
        MFLAG_M = true;
    }
}
void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //std::cout<<yoffset<<std::endl;
    scroll += 2.0f*yoffset;
}
