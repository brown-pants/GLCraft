#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 3) in mat4 instance_matrix;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * instance_matrix * vec4(pos, 1.0);
}