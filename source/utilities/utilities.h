//
// Created by hlahm on 2021-10-12.
//

#ifndef VULKANENGINE2_UTILITIES_H
#define VULKANENGINE2_UTILITIES_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define GLM_MAT4_IDENTITY_INIT  {{1.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 1.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 1.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 1.0f}}

#define GLM_MAT4_IDENTITY GLM_MAT4_IDENTITY_INIT

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace glm;

struct ShaderCode{
    char* code;
    size_t code_size;
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 texCoord;
};

static struct ShaderCode readSPRVFile(std::string filename)
{
    struct ShaderCode shaderCode = {};

    //opens file to read binary
    FILE *fp;
    fp = fopen( filename.c_str(), "rb");

    if(fp == NULL){
        fprintf(stderr,"Failed to open SPR-V file at location: %s\n",filename.c_str());
        exit(1);
    }

    //points to the end of the file and gets size.
    fseek(fp, 0l, SEEK_END);
    size_t fileSize = (size_t)ftell(fp);
    rewind(fp);

    //get file content and return it
    char * sprvContent = static_cast<char *>(malloc(fileSize * sizeof(char)));
    fread(sprvContent, sizeof(char), fileSize, fp);

    fclose(fp);

    shaderCode.code = sprvContent;
    shaderCode.code_size = fileSize;

    return shaderCode;
}

#endif //VULKANENGINE2_UTILITIES_H
