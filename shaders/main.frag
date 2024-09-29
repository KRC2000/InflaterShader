#version 330 core

out vec4 FragColor;

in vec3 Norm;
in vec3 Color;

uniform vec2 Mouse;

void main()
{
    FragColor = vec4(Norm, 1.0f);
} 