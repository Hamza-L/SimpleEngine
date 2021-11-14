
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION

#include "renderer/ve_renderer.cpp"

int main(int argc, char** argv) {

    initEngine();

    renderer.run();

    return 0;
}