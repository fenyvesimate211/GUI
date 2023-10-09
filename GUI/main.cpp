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

// Calculate B-Spline basis functions using Cox-de Boor recursion
float CalculateBasisFunction(int i, int p, float t, const std::vector<float>& knotVector) {
    if (p == 0) {
        if (knotVector[i] <= t && t < knotVector[i + 1]) {
            return 1.0f;
        }
        else {
            return 0.0f;
        }
    }

    float basis1 = 0.0f;
    float basis2 = 0.0f;

    if (knotVector[i + p] - knotVector[i] != 0.0f) {
        basis1 = (t - knotVector[i]) / (knotVector[i + p] - knotVector[i]) * CalculateBasisFunction(i, p - 1, t, knotVector);
    }

    if (knotVector[i + p + 1] - knotVector[i + 1] != 0.0f) {
        basis2 = (knotVector[i + p + 1] - t) / (knotVector[i + p + 1] - knotVector[i + 1]) * CalculateBasisFunction(i + 1, p - 1, t, knotVector);
    }

    return basis1 + basis2;
}

glm::vec3 CalculateBSplineSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPointsU, const std::vector<std::vector<glm::vec3>>& controlPointsV, const std::vector<float>& knotVectorU, const std::vector<float>& knotVectorV) {
    int n = controlPointsU.size() - 1; // Rows
    int m = controlPointsV.size() - 1; // Columns
    

    // Change these values to make the surface different
    int pU = 1;
    int pV = 1;

    glm::vec3 point(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            float basisU = CalculateBasisFunction(i, pU, u, knotVectorU);
            float basisV = CalculateBasisFunction(j, pV, v, knotVectorV);
            point += basisU * basisV * controlPointsU[i][j];
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

    glPointSize(10);

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
    int resolutionU = 25; // Number of steps in the u direction
    int resolutionV = 25; // Number of steps in the v direction

    // Create a 2D vector to store all the surface points
    std::vector<std::vector<glm::vec3>> surfacePoints(resolutionU + 1, std::vector<glm::vec3>(resolutionV + 1));
    std::vector<std::vector<glm::vec3>> surfacePointsBSpline(resolutionU + 1, std::vector<glm::vec3>(resolutionV + 1));

    // Loop through the grid of (u, v) parameter values
    for (int i = 0; i <= resolutionU; i++) {
        float u = static_cast<float>(i) / static_cast<float>(resolutionU);

        for (int j = 0; j <= resolutionV; j++) {
            float v = static_cast<float>(j) / static_cast<float>(resolutionV);

            // Calculate the surface point for the current (u, v)
            glm::vec3 surfacePoint = CalculateBezierSurfacePoint(u, v, controlPoints);


            std::vector<float> knotVectorU = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 12, 12 };
            std::vector<float> knotVectorV = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 12, 12 };

            // Calculate B Spline surface point for the current (u, v)
            glm::vec3 surfacePoint2 = CalculateBSplineSurfacePoint(u, v, controlPoints, controlPoints, knotVectorU, knotVectorV);

            
            // Console log the B Spline points
            // std::cout << "Point at (u=" << i << "/" << resolutionU << ", v=" << j << "/" << resolutionV << "): ";
            // std::cout << "X=" << surfacePoint2.x << ", Y=" << surfacePoint2.y << ", Z=" << surfacePoint2.z << std::endl;
            // Now, 'surfacePoint' contains the 3D coordinates of a point on the Bezier surface
            // You can use this point for rendering or other purposes
            surfacePoints[i][j] = surfacePoint2;
            //surfacePointsBSpline[i][j] = surfacePoint2;
        }
    }

    // Geometry of triangle

    GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };

    GLuint triangle_vao;
    glGenVertexArrays(1, &triangle_vao);
    glBindVertexArray(triangle_vao);

    GLuint triangle_vbo;
    glGenBuffers(1, &triangle_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Geometry of control points

    std::vector<glm::vec3> controlPoints2(16);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            controlPoints2[i * 4 + j] = controlPoints[i][j];
        }
    }

    GLuint cp_vao;
    glGenVertexArrays(1, &cp_vao);
    glBindVertexArray(cp_vao);

    GLuint cp_vbo;
    glGenBuffers(1, &cp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cp_vbo);
    glBufferData(GL_ARRAY_BUFFER, controlPoints2.size() * sizeof(glm::vec3), &controlPoints2[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Geometry of surfice points

    std::vector<glm::vec3> surfacePoints2;
    for (int i = 0; i < resolutionU; i++)
    {
        for (int j = 0; j < resolutionV; j++)
        {
            surfacePoints2.push_back(surfacePoints[i][j]);
            surfacePoints2.push_back(surfacePoints[i + 1][j]);
            surfacePoints2.push_back(surfacePoints[i][j + 1]);

            surfacePoints2.push_back(surfacePoints[i][j + 1]);
            surfacePoints2.push_back(surfacePoints[i + 1][j]);
            surfacePoints2.push_back(surfacePoints[i + 1][j + 1]);
        }
    }

    GLuint sp_vao;
    glGenVertexArrays(1, &sp_vao);
    glBindVertexArray(sp_vao);

    GLuint sp_vbo;
    glGenBuffers(1, &sp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sp_vbo);
    glBufferData(GL_ARRAY_BUFFER, surfacePoints2.size() * sizeof(glm::vec3), &surfacePoints2[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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


        // triangle
        trinagleShader->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
        trinagleShader->EnableShader();
        glBindVertexArray(triangle_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        trinagleShader->DisableShader();

        // control points
        trinagleShader->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
        trinagleShader->EnableShader();
        glBindVertexArray(cp_vao);
        glDrawArrays(GL_POINTS, 0, controlPoints2.size());
        trinagleShader->DisableShader();

        // surface points
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // glPolygonMode for wireframe inspection
        trinagleShader->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
        trinagleShader->EnableShader();
        glBindVertexArray(sp_vao);
        glDrawArrays(GL_TRIANGLES, 0, surfacePoints2.size());
        trinagleShader->DisableShader();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       

        glfwSwapBuffers(window);
    }

   
    glfwTerminate();
    return 0;
}
