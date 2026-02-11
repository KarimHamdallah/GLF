#version 460 core

in vec3 localPos;
out vec4 FragColor;

uniform sampler2D equirectangularMap;

vec2 SampleSphericalMap(vec3 dir)
{
    vec2 uv = vec2(atan(dir.z, dir.x), asin(dir.y));
    uv *= vec2(0.15915f, 0.31831f); // 1/(2*PI) و 1/PI
    uv += 0.5f;
    return uv;
}

void main()
{
    vec2 uv    = SampleSphericalMap(normalize(localPos));
    vec3 color = texture(equirectangularMap, uv).rgb;
    FragColor  = vec4(color, 1.0);
}
