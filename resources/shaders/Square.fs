#version 330 core

in vec2 uv;
out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D shadowMap;
uniform vec3 sunPos;
uniform vec3 moonPos;
uniform vec3 skyColor;
uniform vec3 diveColor;

in vec3 normal;
in vec3 fragPos;
in float visibility;
in vec4 fragPosLightSpace;

float ShadowCalculation(vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if(projCoords.z > 1.0 || any(lessThan(projCoords.xy, vec2(0.0))) || any(greaterThan(projCoords.xy, vec2(1.0))))
        return 0.0;
    
    float cosTheta = clamp(dot(normal, lightDir), 0.0, 1.0);
    float bias = 0.002 * tan(acos(cosTheta));
    bias = clamp(bias, 0.004, 0.008);
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += (projCoords.z - bias) > pcfDepth ? 1.0 : 0.0;
        }    
    }
    
    return shadow / 9.0;
}

void main()
{
    vec4 rgba = texture(tex, uv);
    if(rgba.a < 0.5f) discard;
    
    vec3 squareColor = vec3(rgba);

    vec3 norm = normalize(normal);
    vec3 sunLightDir = normalize(sunPos);
    vec3 moonLightDir = normalize(moonPos);

    float sunDiff = 0.0f;
    float moonDiff = 0.0f;
    float ambientStrength = 0.3;
    float shadow;

    if (sunPos.y > 0.0)
    {
        sunDiff = max(dot(norm, sunLightDir), 0.0);
        shadow = ShadowCalculation(sunLightDir);
    }
    else
    {
        moonDiff = max(dot(norm, moonLightDir), 0.0);
        shadow = ShadowCalculation(moonLightDir);
    }

    vec4 color = vec4((ambientStrength + (1.0 - shadow) * (sunDiff * 0.8 + moonDiff * 0.2)) * squareColor * diveColor, 1.0f);
    FragColor = mix(vec4(skyColor, 1.0f), color, visibility);
}