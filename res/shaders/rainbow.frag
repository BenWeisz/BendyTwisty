#version 410 core

layout(location = 0) out vec4 out_FragmentColor;

smooth in vec3 v_Color;

void main(void) { 
    out_FragmentColor = vec4(v_Color, 1.0);
}