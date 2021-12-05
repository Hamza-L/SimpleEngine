//
// Created by hlahm on 2021-11-07.
//

#ifndef SIMPLEENGINE_KB_INPUT_H
#define SIMPLEENGINE_KB_INPUT_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

static const bool TEXTURE = true;
static bool UP_PRESS = false;
static bool DOWN = false;
static bool RIGHT = false;
static bool LEFT = false;
static bool SHIFT = false;
static bool COM = false;
static float scroll = 0;
static bool ENTER = false;
static bool ENTER_FLAG = true;
static bool ESC = false;
static bool MPRESS_R_Release = true;

//mouse events
static bool MPRESS_R = false;
static bool MFLAG_R = true;
static bool MPRESS_L = false;
static bool MFLAG_L = true;
static bool MPRESS_M = false;
static bool MFLAG_M = true;

//arcball parameter;
static float FIT = 1.5f;
static float GAIN = 2.0f;

#endif //SIMPLEENGINE_KB_INPUT_H
