#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in float instance_vOffset;
layout (location = 3) in mat4 instance_matrix;

out vec2 uv;
uniform mat4 view;
uniform mat4 projection;

uniform int tex2d_count;

void main()
{
    float tex2d_count_f = tex2d_count;
    uv = vec2(texCoord.x,  texCoord.y / tex2d_count_f + instance_vOffset / tex2d_count_f);
    gl_Position = projection * view * instance_matrix * vec4(pos, 1.0);
}