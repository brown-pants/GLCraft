#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 uv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    uv = texCoord;
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}