#version 330 core

layout(location = 1) in vec2 aUV;
layout(location = 0) in vec3 aPos;
layout(location = 2) in int aPal;			// the palette used
layout(location = 3) in int aTexIndex; 

uniform mat4 uVP;

out vec2 vUV;
flat out int vPal;
flat out int vTexIndex; // 👈 NEW

void main() {
    gl_Position = uVP * vec4(aPos, 1);
    vUV = aUV;
    vPal = aPal;
    vTexIndex = aTexIndex; // 👈 pass through
}
