#version 420 core
out vec4 FragColor;

in vec3 outNormal;
in vec4 worldPos;

uniform bool enlight = true;
uniform vec3 color;

void main()
{
    vec3 color = color;
    vec3 lightPos = vec3(0.0, 10.0, -5.0);
    vec3 worldP = vec3(worldPos) / worldPos.w;
    vec3 lv = normalize(lightPos - worldP);
    float f = dot(normalize(lv), outNormal);
    if (enlight){
        if (f > 0)
            color = mix(f*color, color, vec3(0.5));
        else
            color = mix(vec3(0.0), color, vec3(0.5));
    } else {
    }

    FragColor = vec4(color, 1.0);
}