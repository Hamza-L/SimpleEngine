#version 450

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec3 normalForFP;
layout(location = 2) in vec3 positionForFP;
layout(location = 3) in vec3 lightPos1;
layout(location = 4) in vec2 fragTex;
layout(location = 5) in float isTex;
layout(location = 6) in float isNormTex;
layout(location = 7) in mat4 fragM;

layout(location = 0) out vec4 outColor; //final output colour (must also have location) indicates which attachment we are outputting to.

layout(set = 1, binding = 0) uniform sampler2D texSampler;
layout(set = 1, binding = 1) uniform sampler2D normSampler;


void main()
{
    vec4 texColour;
    vec4 normcolour;

    texColour = texture(texSampler, fragTex) * isTex + vec4(fragColour,1.0f) * (1-isTex);
    normcolour = normalize((fragM * (vec4(texture(normSampler, fragTex).xyz,0.5f) * 2.0f - 1.0f))) * isNormTex + vec4(normalForFP,0.0f) * (1-isNormTex);

    vec3 viewDirection = normalize(-positionForFP );
    vec3 lightDirection1 = normalize(lightPos1 - positionForFP );

    vec3 halfVector1 = normalize( lightDirection1 + viewDirection);
    float diffuse1 = max(0.0f,dot( normcolour.xyz, lightDirection1));
    float specular1 = max(0.0f,dot( normcolour.xyz, halfVector1 ) );

    if (diffuse1 == 0.0) {
        specular1 = 0.0;
    } else {
        specular1 = pow( specular1, 16.0f );
    }

    vec3 scatteredLight1 = texColour.xyz * diffuse1;
    vec3 reflectedLight1 = vec3(1.0f,1.0f,1.0f) * specular1 * 0.0f;
    vec3 ambientLight = vec3(0.1f,0.1f,0.1f) * 0.05f;

    outColor = vec4(min( ambientLight  + scatteredLight1 + reflectedLight1 , vec3(1,1,1)), 1.0);
    //outColor = vec4(normalForFP.xyz, 1.0f);
}