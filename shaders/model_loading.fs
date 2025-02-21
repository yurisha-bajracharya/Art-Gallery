#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
int NUM_LIGHTS = 10;
// Lighting uniforms
#define NUM_LIGHTS 5 // Number of lights (one per art piece)
uniform vec3 lightPositions[NUM_LIGHTS];
uniform vec3 viewPos;
uniform vec3 lightColor[NUM_LIGHTS];

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Initialize lighting components
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Loop through all lights
    for (int i = 0; i < NUM_LIGHTS; i++) {
        // Light direction
        vec3 lightDir = normalize(lightPositions[i] - FragPos);

        // Ambient component
        float ambientStrength = 0.2;
        ambient += ambientStrength * lightColor[i];

        // Diffuse component
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += diff * lightColor[i];

        // Specular component
        float specularStrength = 0.3;
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
        specular += specularStrength * spec * lightColor[i];
    }

    // Combine results
    vec3 lighting = ambient + diffuse + specular;

    // Apply texture color
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    // Discard transparent pixels
    if (texColor.a < 0.1)
        discard;

     // Apply gamma correction (optional)
    float gamma = 2.2;
    lighting = pow(lighting * texColor.rgb, vec3(1.0 / gamma));

    FragColor = vec4(lighting, texColor.a);

}
