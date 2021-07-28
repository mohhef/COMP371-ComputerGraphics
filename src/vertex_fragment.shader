#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 Texture;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // model's position with respect to perspective and view

    FragPos = vec3(model * vec4(aPos, 1.0)); // model's position with respect to global axis
    Normal = mat3(transpose(inverse(model))) * aNormal; // model's normal vector
    Texture = aTexture;
}

#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 Texture;

uniform vec3 ourColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D textureObject;
uniform samplerCube depthMap;

uniform float far_plane;
uniform bool shadows;

uniform int textureStatus; // 1 indicates texture is being applied
uniform int shininess;


//float far_plane = 125.0f;
// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
     //float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
     //float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    // PCF
     float shadow = 0.0;
     float bias = 0.5; 
     float samples = 4.0;
     float offset = 0.1;
     for(float x = -offset; x < offset; x += offset / (samples * 0.5))
     {
         for(float y = -offset; y < offset; y += offset / (samples * 0.5))
         {
             for(float z = -offset; z < offset; z += offset / (samples * 0.5))
             {
                 float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // use lightdir to lookup cubemap
                 closestDepth *= far_plane;   // Undo mapping [0;1]
                 if(currentDepth - bias > closestDepth)
                     shadow += 1.0;
             }
         }
     }
     shadow /= (samples * samples * samples);

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


    float shadow = shadows? ShadowCalculation(FragPos) : 0.0;
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuseVal + specularVal)) * color;

    FragColor = vec4(lighting, 1.0f);
}
