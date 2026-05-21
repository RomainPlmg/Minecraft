#version 460 core

uniform sampler2DArray u_texture_array;

in vec3 v_uv_and_layer;
in vec3 v_normal;
in float v_luminosity;

out vec4 frag_color;

void main() {
    vec4 tex_color = texture(u_texture_array, v_uv_and_layer);

    if(tex_color.a < 0.1) {
        discard;
    }

    frag_color = vec4(tex_color.rgb * v_luminosity, 1.0);
}