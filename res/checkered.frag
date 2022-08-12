#version 410 core

uniform vec3 u_LightColor;
uniform vec3 u_DarkColor;
uniform vec2 u_Resolution;

layout(location = 0) out vec4 out_FragmentColor;

in vec3 v_Color;

void main(void) {
    // Shader by Eddie Yepez
    vec2 vUv = gl_FragCoord.xy / u_Resolution.xy;

    float sqrs = 10.0;
    float barX = step(0.5, mod(vUv.x * (sqrs * 0.5), 1.0));
    barX *= step(0.5, mod(vUv.y * (sqrs * 0.5), 1.0));

    float barY = step(0.5, mod(vUv.x * (sqrs * 0.5) + 0.5, 1.0));
    barY *= step(0.5, mod(vUv.y * (sqrs * 0.5) + 0.5, 1.0));

    float strength = barX + barY;
 
    out_FragmentColor = vec4(mix(u_LightColor, u_DarkColor, strength), 1.0);
}