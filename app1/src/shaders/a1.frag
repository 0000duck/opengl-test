#version 420 core
out vec4 FragColor;

uniform vec4 myColor;

void main()
{
    FragColor = myColor;
}