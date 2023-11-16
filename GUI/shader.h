#ifndef SHADER_H
#define SHADER_H


#include <GL/glew.h>
#include <glm/glm.hpp>



class Shader
{
public:

    Shader(const char* vertex_file_path, const char* fragment_file_path);
    ~Shader();
    void EnableShader();
    void DisableShader();

protected:

    GLuint GetUniform(const char* uniformName);

private:
    GLuint shaderProgram;
    GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};



class TriangleShader : public Shader
{
public:

    TriangleShader();
    ~TriangleShader();

    void SetMVP(glm::mat4 MVP);
    void SetColor(glm::vec3 color);
    void SetSelectedPoint(glm::vec3 point);
    void SetSelectedColor(glm::vec3 color);

private:

    GLuint uniform_MVP;
    GLuint uniform_color;
    GLuint uniform_selectedPoint;
    GLuint uniform_selectedColor;
};

class PhongShader : public Shader
{
public:

    PhongShader();
    ~PhongShader();

    void SetMVP(glm::mat4 MVP);
    void SetLightPosition(glm::vec3 color);
    void SetViewPos(glm::vec3 color);
    void SetObjectColor(glm::vec3 color);

private:

    GLuint uniform_MVP;
    GLuint uniform_lightPosition;
    GLuint uniform_viewPos;
    GLuint uniform_objectColor;
};




#endif