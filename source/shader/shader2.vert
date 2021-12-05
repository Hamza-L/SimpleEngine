#version 450    //version of GLSL (4.5)

//array for triangle that fills screen
vec2 positions[3] = {
vec2(3.0, -1.0),
vec2(-1.0, -1.0),
vec2(-1.0, 3.0)
};

void main()
{
    gl_Position = vec4(positions[gl_VertexIndex], 0.0f, 1.0f);
}
