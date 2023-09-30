#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "camera.h"
#include <iostream>

// Calculating points for Bezier

    // Calculate binomial coefficient C(n, k)
int BinomialCoefficient(int n, int k) {
    if (k < 0 || k > n) {
        return 0;
    }

    int result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }

    return result;
}

glm::vec3 CalculateBezierSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints) {
    int n = controlPoints.size() - 1; // Rows
    int m = controlPoints[0].size() - 1; // Columns

    glm::vec3 point(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            int coeff = BinomialCoefficient(n, i) * BinomialCoefficient(m, j);
            float term = coeff * std::pow(u, i) * std::pow(1 - u, n - i) * std::pow(v, j) * std::pow(1 - v, m - j);
            point += term * controlPoints[i][j];
        }
    }

    return point;
}

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

    // Control Points

    std::vector<std::vector<glm::vec3>> controlPoints(4, std::vector<glm::vec3>(4));

    // Define the control points
    controlPoints[0][0] = glm::vec3(-1.5f, -1.5f, 0.0f);
    controlPoints[0][1] = glm::vec3(-0.5f, -1.5f, 0.0f);
    controlPoints[0][2] = glm::vec3(0.5f, -1.5f, 0.0f);
    controlPoints[0][3] = glm::vec3(1.5f, -1.5f, 0.0f);

    controlPoints[1][0] = glm::vec3(-1.5f, -0.5f, 0.0f);
    controlPoints[1][1] = glm::vec3(-0.5f, -0.5f, 1.0f);
    controlPoints[1][2] = glm::vec3(0.5f, -0.5f, 1.0f);
    controlPoints[1][3] = glm::vec3(1.5f, -0.5f, 0.0f);

    controlPoints[2][0] = glm::vec3(-1.5f, 0.5f, 0.0f);
    controlPoints[2][1] = glm::vec3(-0.5f, 0.5f, 1.0f);
    controlPoints[2][2] = glm::vec3(0.5f, 0.5f, 1.0f);
    controlPoints[2][3] = glm::vec3(1.5f, 0.5f, 0.0f);

    controlPoints[3][0] = glm::vec3(-1.5f, 1.5f, 0.0f);
    controlPoints[3][1] = glm::vec3(-0.5f, 1.5f, 0.0f);
    controlPoints[3][2] = glm::vec3(0.5f, 1.5f, 0.0f);
    controlPoints[3][3] = glm::vec3(1.5f, 1.5f, 0.0f);

    // Define the resolution of the grid (e.g., number of steps in u and v)
    int resolutionU = 50; // Number of steps in the u direction
    int resolutionV = 50; // Number of steps in the v direction

    // Create a 2D vector to store all the surface points
    std::vector<std::vector<glm::vec3>> surfacePoints(resolutionU + 1, std::vector<glm::vec3>(resolutionV + 1));

    // Loop through the grid of (u, v) parameter values
    for (int i = 0; i <= resolutionU; i++) {
        float u = static_cast<float>(i) / static_cast<float>(resolutionU);

        for (int j = 0; j <= resolutionV; j++) {
            float v = static_cast<float>(j) / static_cast<float>(resolutionV);

            // Calculate the surface point for the current (u, v)
            glm::vec3 surfacePoint = CalculateBezierSurfacePoint(u, v, controlPoints);

            std::cout << "Point at (u=" << i << "/" << resolutionU << ", v=" << j << "/" << resolutionV << "): ";
            std::cout << "X=" << surfacePoint.x << ", Y=" << surfacePoint.y << ", Z=" << surfacePoint.z << std::endl;
            // Now, 'surfacePoint' contains the 3D coordinates of a point on the Bezier surface
            // You can use this point for rendering or other purposes
            surfacePoints[i][j] = surfacePoint;
        }
    }
   
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
