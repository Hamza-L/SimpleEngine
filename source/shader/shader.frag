#version 450

layout(location = 0) in vec4 fragColour;

layout(location = 0) out vec4 outColor; //final output colour (must also have location) indicates which attachment we are outputting to.

void main()
{
    outColor = vec4(fragColour);
}