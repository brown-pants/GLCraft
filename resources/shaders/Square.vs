#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in float instance_vOffset;
layout (location = 3) in mat4 instance_matrix;

out vec2 uv;
uniform mat4 view;
uniform mat4 projection;
uniform int tex2d_count;
uniform float density;
uniform float gradient;

out vec3 normal;
out vec3 fragPos;

out float visibility;

void main()
{
    //Light
    normal = mat3(transpose(inverse(instance_matrix))) * vec3(0.f, 0.f, -1.f);
    fragPos = vec3(instance_matrix * vec4(pos, 1.0));

    //Fog
    vec4 positionRelativeToCamera = view * instance_matrix * vec4(pos, 1.0);
    float distance = length(positionRelativeToCamera.xyz);
    visibility = exp(-pow((distance * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    //Position
    float tex2d_count_f = tex2d_count;
    uv = vec2(texCoord.x,  texCoord.y / tex2d_count_f + instance_vOffset / tex2d_count_f);
    gl_Position = projection * positionRelativeToCamera;
}