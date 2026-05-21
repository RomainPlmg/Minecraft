#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in float a_tex_id;
layout (location = 3) in vec3 a_normal;
layout (location = 4) in float a_luminosity;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 v_uv_and_layer;
out vec3 v_normal;
out float v_luminosity;

void main() {
    v_uv_and_layer.xy = a_uv;
    v_uv_and_layer.z = a_tex_id;

    v_normal = a_normal;
    v_luminosity = a_luminosity;
    gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);
}