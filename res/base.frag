#version 410 core

in vec3 v_Color;
layout(location = 0) out vec4 out_FragmentColor;
void main(void) {
    out_FragmentColor = vec4(v_Color, 1.0);
}