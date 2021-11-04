
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION

#include "renderer/ve_renderer.cpp"

int main() {

    initEngine();
    renderer.run();

    return 0;
}