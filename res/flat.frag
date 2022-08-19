#version 410 core

uniform vec3 u_FlatColor;

layout(location = 0) out vec4 out_FragmentColor;

void main(void) { 
    out_FragmentColor = vec4(u_FlatColor, 1.0);
}