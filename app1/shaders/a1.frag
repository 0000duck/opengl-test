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

#define PI 3.14159265

/* (c) learnopengl.com */
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 color)
{
    vec3 F0 = mix(vec3(0.04), color, 0);
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

/* */


vec3 CookTorrance(vec3 n, vec3 v, vec3 l, float a, vec3 F) {
    float k = pow(a+1.0, 2.0)/8.0;
    float D = DistributionGGX(n, normalize(l+v), a);
    float G = GeometrySmith(n, v, l, a);
    return D*G*F/4.0/max(0.0001, dot(v,n)* dot(l,n));
}

vec3 processPointLight(PointLight light, vec3 worldPos, vec3 normal) {
    vec3 outColor = vec3(0.0);
    vec3 distV = light.position - worldPos;
    vec3 lv = normalize(distV);
    float dist = length(distV);
    float noiseVal = texture(noise, texCoord).x;
    float attenuation = attFactor(light.attenuation, dist);
    vec3 viewV = normalize(viewerPos - worldPos);
    float clv = max(0.0, dot(lv, normal));
    vec3 color = vec3(0.85+0.1*noiseVal,0.85+0.1*(1.0-noiseVal),0.85+0.1*(1.0-noiseVal))/5.0;
    vec3 F = fresnelSchlick(dot(viewV, normalize(viewV+lv)), color);

    vec3 res = vec3(0.0);
    res += clv * color * (1.0 - F)/PI;
    res += clv * CookTorrance(normal, viewV, lv, 0.05 + 0.5 *noiseVal, F);


    return res;
}


void main()
{
    vec3 outColor = vec3(0.0);
    vec3 worldPos = vec3(worldPos) / worldPos.w;
    if (enlight) {
        for(int i = 0; i < numPointLights; i++) {
            outColor += processPointLight(pointLights[i], worldPos, normalize(outNormal));
        }
    } else {
        outColor = color;
    }
    outColor = outColor / (outColor + vec3(1.0));
    outColor = pow(outColor, vec3(1.0/2.2));

    FragColor = vec4(outColor, 1.0);
}