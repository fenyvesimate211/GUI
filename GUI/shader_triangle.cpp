#include "shader.h"


TriangleShader::TriangleShader() : Shader{ "triangle.vs", "triangle.fs" }
{
    uniform_MVP = GetUniform("MVP");
    uniform_color = GetUniform("color");
    uniform_selectedPoint = GetUniform("selectedPoint");
    uniform_selectedColor = GetUniform("selectedColor");
}

TriangleShader::~TriangleShader() {}


void TriangleShader::SetMVP(glm::mat4 MVP)
{
    EnableShader();
    glUniformMatrix4fv(uniform_MVP, 1, GL_FALSE, &MVP[0][0]);
    DisableShader();
}

void TriangleShader::SetColor(glm::vec3 color)
{
    EnableShader();
    glUniform3fv(uniform_color, 1, &color[0]);
    DisableShader();
}

void TriangleShader::SetSelectedPoint(glm::vec3 point)
{
    EnableShader();
    glUniform3fv(uniform_selectedPoint, 1, &point[0]);
    DisableShader();
}

void TriangleShader::SetSelectedColor(glm::vec3 color)
{
    EnableShader();
    glUniform3fv(uniform_selectedColor, 1, &color[0]);
    DisableShader();
}
