#version 450    //version of GLSL (4.5)

//binding 0 for ins (changes for every vertex)
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoord;

//outputs
layout(location = 0) out vec3 fragColour; //output colour for vertex (location is required)
layout(location = 1) out vec3 normalForFP;
layout(location = 2) out vec3 positionForFP;
layout(location = 3) out vec3 lightPos1;
layout(location = 4) out vec2 fragTex;
layout(location = 5) out float isTex;
layout(location = 6) out float isNormTex;
layout(location = 7) out mat4 fragM;

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

    mat4 viewMat = {
    cam.view[0],
    cam.view[1],
    cam.view[2],
    cam.view[3] * (1 - pObj.isEnvMap) + vec4(0.0f,0.0f,0.0f,1.0f) * (pObj.isEnvMap)
    };
    gl_Position = cam.projection * viewMat * pObj.model * vec4(position.x,position.y,position.z, 1.0f);
    //gl_Position = vec4(position.x,-position.y,position.z, 1.0f);
    fragColour = color;

    vec4 tempNorm = cam.view * pObj.model * vec4(normal, 0.0f);
    normalForFP = normalize(tempNorm.xyz);

    vec4 tempPos = cam.view * pObj.model * vec4(position, 1.0);
    positionForFP = tempPos.xyz;

    vec4 tempLPos1 = cam.view * light.light1;
    lightPos1 = tempLPos1.xyz;

    fragTex = texCoord;
    isTex = pObj.isTex;
    isNormTex = pObj.isNormTex;

    //model reconstructed from up and normal vector
    vec3 up = vec3(0.0f,1.0f,0.0f);
    vec3 u = cross(up,normal);
    mat4 tempM = mat4(
    vec4(u,0.0f),
    vec4(up,0.0f),
    vec4(normal,0.0f),
    vec4(0.0f,0.0f,0.0f,1.0f)
    );

    mat4 Mnorm;
    fragM = cam.view * pObj.model * tempM;

}