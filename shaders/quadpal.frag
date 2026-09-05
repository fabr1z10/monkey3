#version 330 core

in vec2 vUV;
flat in int vPal;
flat in int vTexIndex;

out vec4 FragColor;

uniform usampler2D uTextures[16];
uniform sampler2D tex_pal;

void main()
{

    uint index = texture(uTextures[vTexIndex], vUV).r;
    vec4 texColor = texelFetch(tex_pal, ivec2(int(index), vPal), 0);
    if (texColor.a < 0.5) {
        discard;
    }
    FragColor=texColor;

}