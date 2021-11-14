#version 450    //version of GLSL (4.5)

//binding 0 for ins (changes for every vertex)
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

//outputs
layout(location = 0) out vec4 fragColour; //output colour for vertex (location is required)

//binding 0 for uniforms (does no change for every vertex)
layout( set = 0, binding = 0) uniform Cam{
    mat4 projection;
    mat4 view;
} cam;

layout( set = 0, binding = 1) uniform Light{
    vec4 light1;
} light;

//push constant
layout(push_constant) uniform PushObj{
    mat4 model;
    float isTex;
    float isNormTex;
    float isEnvMap;
}pObj;

void main()
{
    gl_PointSize = 5.0f;
    gl_Position = pObj.model * vec4(position.x,position.y,position.z, 1.0f);
    //gl_Position = vec4(position.x,-position.y,position.z, 1.0f);
    float alpha = 1;
    if(pObj.isTex>0 && pObj.isTex<1.0f){
        alpha = pObj.isTex;
    }
    fragColour = vec4(color,alpha);

}