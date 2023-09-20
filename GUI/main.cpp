#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <stdio.h>

#include "shader.h"

int main() {

    // GLFW

    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    // Shader

    Shader* TrinagleShader = new Shader("triangle.vs","triangle.fs");

    // Geometry

    GLuint vao;
    GLuint vbo;

    GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(0);
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

   
    // Rendering
   
    TrinagleShader->EnableShader();

    while (!glfwWindowShouldClose(window)) {
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
       
        glfwPollEvents();
        
        glfwSwapBuffers(window);
    }

   
    glfwTerminate();
    return 0;
}
