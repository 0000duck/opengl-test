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

    vec3 attenuation;
};
#define MAX_POINT_LIGHTS 20
uniform PointLight pointLights[MAX_POINT_LIGHTS];

float attFactor (vec3 att, float dist){
  return 1.0/(((att.z*dist)+att.y)*dist+att.x);
}

vec3 processPointLight(PointLight light, vec3 worldPos) {
    vec3 outColor = vec3(0.0);
    vec3 distV = light.position - worldPos;
    vec3 lv = normalize(distV);
    float dist = length(distV);

    float attenuation = attFactor(light.attenuation, dist);

    float clv = dot(lv, outNormal);
    outColor += light.ambient * color;
    if (clv > 0) {
        outColor += (clv * light.diffuse) * color * attenuation;
    }
    return outColor;
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