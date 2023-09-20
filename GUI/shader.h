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

private:

    GLuint uniform_MVP;
};




#endif