#ifndef SHADER_H
#define SHADER_H


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
#endif