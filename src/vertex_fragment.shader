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
out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //calculate pos in world space (i.e. without view & projection)
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model)))*aNormal;
    
    texCoord = aTexture;
}

#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

uniform vec3 ourColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D textureLoc;

void main()
{
    vec4 texColor = texture(textureLoc, texCoord);

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
    vec3 reflectDirection = reflect(-lightDirection, norm);
    int shininess = 32;
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specularVal = specularFactor * spec * lightColor;

    vec3 result = (ambientVal+diffuseVal+specularVal) * ourColor * texColor.rgb;
    FragColor = vec4(result,1.0f);
}
