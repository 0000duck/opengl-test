#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    gl_Position = mvp * gl_Position;
}