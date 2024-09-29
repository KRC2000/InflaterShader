#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 Norm;
out vec3 Color;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform vec2 Mouse;
uniform vec2 WinSize;

void main()
{
    Norm = aNorm;

    vec3 aPos_ = aPos;
    vec4 pos = p * v * m * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    
    vec2 NDC = (pos.xy / pos.w) * 0.5 + 0.5;
    
    vec2 ScreenPos = NDC * WinSize;
    
    float dist = distance(ScreenPos, Mouse);
    
    // aPos_.z += clamp(10 / dist, 0, 1);
    vec3 increment = Norm / (dist/10);
    increment = min(vec3(0.2, 0.2, 0.2), increment);
    increment = max(vec3(-0.2, -0.2, -0.2), increment);
    aPos_ += increment;

    gl_Position = p * v * m * vec4(aPos_.x, aPos_.y, aPos_.z, 1.0);


    Color = vec3(clamp(10/dist, 0, 1), 0, 0);
}