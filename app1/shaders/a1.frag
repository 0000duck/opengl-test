#version 420 core
out vec4 FragColor;

in vec3 outNormal;
in vec4 worldPos;
in vec2 texCoord;

uniform bool enlight = true;
uniform vec3 color;
uniform int numPointLights;
uniform vec3 viewerPos;

uniform sampler2D noise;

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

vec3 project(vec3 v, vec3 n) {
    return v - dot(v, n) * n;
}

float W(float a) {
    return 1.0 + (1.0 - abs(a)) * 4.0;
}

vec3 processPointLight(PointLight light, vec3 worldPos) {
    vec3 outColor = vec3(0.0);
    vec3 distV = light.position - worldPos;
    vec3 lv = normalize(distV);
    float dist = length(distV);
    vec4 noiseVal = texture(noise, texCoord);
    float attenuation = attFactor(light.attenuation, dist);

    float clv = dot(lv, outNormal);
    outColor += light.ambient * color;
    if (clv > 0) {
        outColor += clv * light.diffuse * color * attenuation;
        vec3 viewV = normalize(viewerPos - worldPos);
        vec3 h = normalize(lv+viewV);
        float spec = W(clv) * pow(dot(h, outNormal), 2*10.0);
        outColor += spec  *light.specular * color * attenuation;
    }
    outColor.r *= pow(noiseVal.x + 0.5, 3.0);
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