#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
    FragPos = aPos;
    Normal = aNormal;
}