#version 410 core

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;

uniform vec3 u_FlatColor;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;

layout(location = 0) out vec4 out_FragmentColor;

in vec3 v_SurfaceNormal;
in vec3 v_FragPos;

void main(void) { 
    float ambient = u_AmbientStrength;
    
    vec3 normal = normalize(v_SurfaceNormal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);

    float diffuse = max(0, dot(normal, lightDir));

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 100) * u_SpecularStrength;

    vec3 total = (ambient + diffuse + specular) * (u_FlatColor * u_LightColor);

    out_FragmentColor = vec4(total, 1.0);
}