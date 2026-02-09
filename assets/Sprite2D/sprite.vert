#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;

uniform mat4 proj;
uniform mat4 model;

void main()
{
    gl_Position = proj * model * vec4(aPos.xyz, 1.0);
    vTexCoords = aTexCoords;
}
