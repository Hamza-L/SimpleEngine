#version 450    //version of GLSL (4.5)

layout(input_attachment_index = 0, binding = 0) uniform subpassInput inputColor; //color output from subpass 1
layout(input_attachment_index = 1, binding = 1) uniform subpassInput inputDepth; //depth output from subpass 1

layout(push_constant) uniform IA_PushObj{
    float x_slider;
    float fog_pow;
    float fog_shift;
}ia_pObj;

layout(location = 0) out vec4 color;

void main()
{
    int xHalf = 1200/2;
    if(gl_FragCoord.x > 0.0f)
    {
        float lowerBound = 0.99f;
        float upperBound = 1.0f;

        float depth = subpassLoad(inputDepth).r;
        float depthColorScaled = 1.0f - ((depth-lowerBound) / (upperBound - lowerBound));
        depthColorScaled = clamp(depthColorScaled-ia_pObj.fog_shift, 0.0f, 1.0f);
        depthColorScaled = pow(depthColorScaled, ia_pObj.fog_pow);

        color = vec4(depthColorScaled * depthColorScaled * depthColorScaled * subpassLoad(inputColor).rgb,1.0f);
    } else
    {
        color = subpassLoad(inputColor).rgba;
    }

}