#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 Texture;
out vec4 FragPostLightSpace;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // model's position with respect to perspective and view

    FragPos = vec3(model * vec4(aPos, 1.0)); // model's position with respect to global axis
    Normal = mat3(transpose(inverse(model))) * aNormal; // model's normal vector
    Texture = aTexture;
    FragPostLightSpace = lightSpaceMatrix* vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 Texture;
in vec4 FragPostLightSpace;

uniform vec3 ourColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D textureObject;
uniform sampler2D shadowMap;

uniform int textureStatus; // 1 indicates texture is being applied
uniform int shininess;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{
    vec3 color;

    if (textureStatus == 1)
    {
        // determine result with texture applied
        color = ourColor * texture(textureObject, Texture).rgb;
    }
    else
    {
        // determine result without texture applied
        color = ourColor;
    }
    vec3 ambient = 0.15 * color;
    //ambient
    float ambientFactor = 0.1;
    vec3 ambientVal = ambientFactor * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuseVal = diff * lightColor;

    // specular
    float specularFactor = 0.5;
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    //vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(norm, halfwayDirection), 0.0), shininess);
    vec3 specularVal = specularFactor * spec * lightColor;


    float shadow = ShadowCalculation(FragPostLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuseVal + specularVal)) * color;

    FragColor = vec4(lighting, 1.0f);
}
