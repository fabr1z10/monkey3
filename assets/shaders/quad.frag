#version 330 core

in vec2 vUV;
in vec4 vColor;
flat in float vTexIndex; // 👈 NEW

out vec4 FragColor;

uniform sampler2D uTextures[16];

void main()
{
    int index = int(vTexIndex);
    vec4 texColor = texture(uTextures[index], vUV);
    FragColor = texColor * vColor;
}