#version 420 core
out vec4 FragColor;

in vec3 outNormal;
in vec4 worldPos;

void main()
{
    vec3 color = vec3(0.5);
    vec3 lightPos = vec3(0.0, 10.0, -5.0);
    vec3 worldP = vec3(worldPos) / worldPos.w;
    vec3 lv = lightPos - worldP;
    float f = dot(lv, outNormal);
    color = mix(f*color, color, vec3(0.5));
    FragColor = vec4(color, 1.0);
}