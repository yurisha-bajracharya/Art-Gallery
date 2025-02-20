#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

// Lighting uniforms
uniform vec3 lightPos;   
uniform vec3 viewPos;    
uniform vec3 lightColor; 

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Increase ambient strength to prevent dark areas
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting (Shiny effect)
    float specularStrength = 0.6;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = (ambient + diffuse + specular);

    // Use texture with alpha
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // Discard transparent pixels
    if (texColor.a < 0.1)
        discard;

    vec3 result = lighting * texColor.rgb;

    // Preserve alpha transparency
    FragColor = vec4(result, texColor.a);
}
