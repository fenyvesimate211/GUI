#version 410

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main() {
    vec3 lightAmbient = vec3(1.0, 1.0, 1.0) * 0.1;
    vec3 lightDiffuse = vec3(1.0, 1.0, 1.0) * 0.6;
    vec3 lightSpecular = vec3(1.0, 1.0, 1.0)* 0.6;
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // Double sided rendering: normals alway face to viewer
    if (dot(norm, viewDir) < 0.0) {
        norm = -norm;
    }

    vec3 ambient = lightAmbient * objectColor;

    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * objectColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    if (diff == 0.0) // No reflection on unlit faces
        spec = 0.0;
    vec3 specular = lightSpecular * spec;
    
    vec3 result = (ambient + diffuse + specular);
    
    FragColor = vec4(result, 1.0);
}