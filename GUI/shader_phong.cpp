#include "shader.h"


PhongShader::PhongShader() : Shader{ "phong.vs", "phong.fs" }
{
    uniform_MVP = GetUniform("MVP");
    uniform_lightPosition = GetUniform("lightPosition");
    uniform_viewPos = GetUniform("viewPos");
    uniform_objectColor = GetUniform("objectColor");
}

PhongShader::~PhongShader() {}


void PhongShader::SetMVP(glm::mat4 MVP)
{
    EnableShader();
    glUniformMatrix4fv(uniform_MVP, 1, GL_FALSE, &MVP[0][0]);
    DisableShader();
}

void PhongShader::SetLightPosition(glm::vec3 color)
{
    EnableShader();
    glUniform3fv(uniform_lightPosition, 1, &color[0]);
    DisableShader();
}

void PhongShader::SetViewPos(glm::vec3 color)
{
    EnableShader();
    glUniform3fv(uniform_viewPos, 1, &color[0]);
    DisableShader();
}

void PhongShader::SetObjectColor(glm::vec3 color)
{
    EnableShader();
    glUniform3fv(uniform_objectColor, 1, &color[0]);
    DisableShader();
}
