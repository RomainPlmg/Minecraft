#version 410 core

uniform sampler2D u_atlas;

in vec3 v_pos;
in vec2 v_uv;
in vec3 v_normal;
in float v_luminosity;

out vec4 frag_color;

void main() {
    vec4 tex_color = texture(u_atlas, v_uv);

    if(tex_color.a < 0.1) {
        discard;
    }

    frag_color = vec4(tex_color.rgb * v_luminosity, 1.0);
}