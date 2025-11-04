#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 sunPos;
uniform vec3 moonPos;

in vec3 normal;
in vec3 fragPos;

in float visibility;

uniform vec3 skyColor;

void main()
{
    vec4 rgba = texture(tex, uv) * vec4(0.3, 0.5, 0.7, 1.0f);
    
    vec3 squareColor = vec3(rgba);

    vec3 norm = normalize(normal);
    vec3 sunLightDir = normalize(sunPos);
    vec3 moonLightDir = normalize(moonPos);

    float sunDiff = 0.0f;
    float moonDiff = 0.0f;
    float ambientStrength = 0.3;

    if (sunPos.y > 0.0)
    {
        sunDiff = max(dot(norm, sunLightDir), 0.0);
    }
    if (moonPos.y > 0.0)
    {
        moonDiff = max(dot(norm, moonLightDir), 0.0);
    }

    vec4 color = vec4((ambientStrength + sunDiff * 0.8 + moonDiff * 0.2) * squareColor, rgba.a);
    FragColor = mix(vec4(skyColor, 1.0f), color, visibility);
}