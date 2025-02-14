#version 330 core

layout (location = 0) in vec2 pos;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(pos, -0.2f, 1.0f);
}