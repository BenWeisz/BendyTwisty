#version 410 core

uniform mat4 u_MVP;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

void main(void) {
    gl_Position = u_MVP * vec4(in_Position, 1.0);
}