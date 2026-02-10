#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

out vec3 vNormals;
out vec2 vTexCoords;

uniform mat4 view_proj;
uniform mat4 model;

void main()
{
    gl_Position = view_proj * model * vec4(aPos.xyz, 1.0);
    vNormals = aNormals;
    vTexCoords = aTexCoords;
}
