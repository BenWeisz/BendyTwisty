#version 410 core

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_NormalModel;

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

flat out vec3 v_SurfaceNormal;
out vec3 v_FragPos;

void main(void) {
    gl_Position = u_MVP * vec4(in_Position, 1.0);
    v_SurfaceNormal = vec3(u_NormalModel * vec4(normalize(in_Normal), 1.0));
    v_FragPos = vec3(u_Model * vec4(in_Position, 1.0));
}