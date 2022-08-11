#version 410 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;

uniform mat4 u_Test;

out vec3 v_Color;
void main(void) {
    gl_Position = u_Test * vec4(in_Position, 1.0);
    v_Color = in_Color;
}