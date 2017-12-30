#version 420 core
out vec4 FragColor;

in vec3 outNormal;
in vec4 worldPos;

uniform bool enlight = true;
uniform vec3 color;
uniform int numPointLights;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float c;
    float l;
    float q;
};
#define MAX_POINT_LIGHTS 20
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 processPointLight(PointLight light, vec3 worldPos) {
    vec3 outColor = vec3(0.0);
    vec3 distV = light.position - worldPos;
    vec3 lv = normalize(distV);
    float dist = length(distV);

    float attenuationI = light.c + dist * light.l + (dist * dist) * light.q;

    float clv = dot(lv, outNormal);
    outColor += light.ambient * color;
    if (clv > 0) {
        outColor += (clv * light.diffuse) * color;
    }
    return outColor / attenuationI;
}

void main()
{
    vec3 outColor = vec3(0.0);
    vec3 worldPos = vec3(worldPos) / worldPos.w;
    if (enlight) {
        for(int i = 0; i < numPointLights; i++) {
            outColor += processPointLight(pointLights[i], worldPos);
        }
    } else {
        outColor = color;
    }

    FragColor = vec4(outColor, 1.0);
}