#version 460 core

out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D Texture0;

void main()
{
    FragColor = texture(Texture0, vTexCoords);
}
