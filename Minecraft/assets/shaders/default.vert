#version 330 core

layout (location = 0) in vec3 aPos; // The position variable has attribute position 0
layout (location = 1) in vec3 aColor; // The position variable has attribute position 1
layout (location = 2) in vec2 aUV; // The position variable has attribute position 2

out vec3 outColor;
out vec2 outUV;

uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * vec4(aPos, 1.0);
    outColor = aColor;
    outUV = aUV;
}