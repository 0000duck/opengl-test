#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 normalm;

out vec3 outNormal;
out vec4 worldPos;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    //color = mix(aPos, vec3(1.0), vec3(0.5));
    outNormal = normalm * inNormal;
    worldPos = model * vec4(aPos, 1.0);
}