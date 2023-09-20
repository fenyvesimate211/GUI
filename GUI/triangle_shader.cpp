#include "shader.h"


TriangleShader::TriangleShader() : Shader{ "triangle.vs", "triangle.fs" }
{
    uniform_MVP = GetUniform("MVP");
}

TriangleShader::~TriangleShader() {}


void TriangleShader::SetMVP(glm::mat4 MVP)
{
    EnableShader();
    glUniformMatrix4fv(uniform_MVP, 1, GL_FALSE, &MVP[0][0]);
    DisableShader();
}