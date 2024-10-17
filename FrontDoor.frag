#version 330 core
out vec4 FragColor; // Returns FragColor

in vec3 Normal; // Receives normal
in vec3 FragPos; // Receives FragPos

uniform vec3 lightPos; // Receives lightPos uniform
uniform vec3 viewPos; // Receives viewPos uniform
uniform vec3 lightColor; // Receives lightColor uniform
uniform vec3 objectColor; // Receives objectColor uniform
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

void main() {
    // ambient
    vec3 ambient = ambientStrength * lightColor; // Sets ambient
    
    // diffuse
    vec3 norm = normalize(Normal); // Normalizes normal
    vec3 lightDir = normalize(lightPos - FragPos); // Gets lightDir
    float diff = max(dot(norm, lightDir), 0.0); // Gets diff
    vec3 diffuse = diff * lightColor; // Sets diffuse

    // specular
    vec3 viewDir = normalize(viewPos - FragPos); // Get viewDir
    vec3 reflectDir = reflect(-lightDir, norm); // Get reflectDir
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); // Get spec
    vec3 specular = specularStrength * spec * lightColor; // Set specular

    vec3 result = (ambient + diffuse + specular) * objectColor; // Calculate result
    FragColor = vec4(result, 1.0f); // Set FragColor output
}

