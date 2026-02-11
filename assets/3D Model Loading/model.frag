#version 460 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;

in vec3 vNormals;
in vec2 vTexCoords;
flat in int vTextureIndex;

// Textures Shader Storage Buffer
layout(std430, binding = 0) buffer TextureHandles
{
    uvec2 handles[];
};

void main()
{
    sampler2D albedo = sampler2D(handles[vTextureIndex]);
    FragColor = texture(albedo, vTexCoords);
}
