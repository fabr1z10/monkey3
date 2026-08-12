#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uScene;

void main()
{
    vec4 color = texture(uScene, vUV);

    float line = mod(floor(gl_FragCoord.y), 4.0);

    float scanline = (line < 2.0) ? 0.60 : 1.0;

    FragColor = color * scanline;
}