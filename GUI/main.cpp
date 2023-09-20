#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <stdio.h>

#include "shader.h"
#include "camera.h"

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

    window = glfwCreateWindow(1280, 720, "Hello Triangle", NULL, NULL);
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

    // Camera

    Camera* camera = new Camera();
    camera->horizontalAngle = glm::pi<float>(); // Looking in -Z direction.
    camera->position = glm::vec3(0.0f, 0.0f, 5.0f);

    // Shader

    TriangleShader* trinagleShader = new TriangleShader();
   
    // Rendering


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        camera->OnRender(window);
        glm::mat4 model = glm::mat4(1.0f); // identity matrix
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 MVP = projection * view * model;
        trinagleShader->SetMVP(MVP);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        
        trinagleShader->EnableShader();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        trinagleShader->DisableShader();
       

        glfwSwapBuffers(window);
    }

   
    glfwTerminate();
    return 0;
}
