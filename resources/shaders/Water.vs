#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in mat4 instance_matrix;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;
uniform float texOffset;

out float visibility;
const float density = 0.005;
const float gradient = 9.5;

void main()
{
    //Light
    normal = vec3(0.f, 1.f, 0.f);
    fragPos = vec3(instance_matrix * vec4(pos, 1.0));

    //Fog
    vec4 positionRelativeToCamera = view * instance_matrix * vec4(pos, 1.0);
    float distance = length(positionRelativeToCamera.xyz);
    visibility = exp(-pow((distance * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);
    
    //Position
    uv = vec2(texCoord.x,  texCoord.y / 16.0 + texOffset / 16.0);
    gl_Position = projection * positionRelativeToCamera;
}