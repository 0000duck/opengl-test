#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 mvp;

out vec3 color;
out vec3 outNormal;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position = mvp * gl_Position;
    color = mix(aPos, vec3(1.0), vec3(0.5));
    outNormal = inNormal;
}