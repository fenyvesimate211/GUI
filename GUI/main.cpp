#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "surface.h"
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"



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

    // Initialize Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // Camera

    Camera* camera = new Camera();
    camera->horizontalAngle = glm::pi<float>(); // Looking in -Z direction.
    camera->position = glm::vec3(0.0f, 0.0f, 5.0f);

    // Shader

    TriangleShader* trinagleShader = new TriangleShader();
    PhongShader* phongShader = new PhongShader();


    // Surface and ControlPoints
    Surface* surface = new NURBSSurface();
    surface->CalculateSurfacePoints();
    

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
            controlPoints2[i * 4 + j] = surface->controlPoints[i][j];
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
    std::vector<glm::vec3> normals;
    for (int i = 0; i < surface->resolutionU; i++)
    {
        for (int j = 0; j < surface->resolutionV; j++)
        {
            glm::vec3 v1 = surface->surfacePoints[i][j];
            glm::vec3 v2 = surface->surfacePoints[i + 1][j];
            glm::vec3 v3 = surface->surfacePoints[i][j + 1];
            glm::vec3 v4 = surface->surfacePoints[i + 1][j + 1];

            surfacePoints2.push_back(v1);
            surfacePoints2.push_back(v2);
            surfacePoints2.push_back(v3);

            surfacePoints2.push_back(v3);
            surfacePoints2.push_back(v2);
            surfacePoints2.push_back(v4);

            glm::vec3 normal1 = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            normals.push_back(normal1);
            normals.push_back(normal1);
            normals.push_back(normal1);

            glm::vec3 normal2 = glm::normalize(glm::cross(v3 - v4, v2 - v4));
            normals.push_back(normal2);
            normals.push_back(normal2);
            normals.push_back(normal2);
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

    GLuint sp_nbo;
    glGenBuffers(1, &sp_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, sp_nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Rendering

    int selectedPointU = 0;
    int selectedPointV = 0;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow();

        // Define window components
        ImGui::Begin("Select a surface");
        const char* items[] = { "Bezier", "Bspline", "Nurbs"};
        static int item_current = 0;
        ImGui::Combo("Surface", &item_current, items, IM_ARRAYSIZE(items));
        ImGui::SeparatorText("Edit control points");
        ImGui::Text("U:       ");
        ImGui::SameLine();
        ImGui::InputInt("##U", &selectedPointU);
        ImGui::Text("V:       ");
        ImGui::SameLine();
        ImGui::InputInt("##V", &selectedPointV);
        selectedPointU = glm::clamp(selectedPointU, 0, 3);
        selectedPointV = glm::clamp(selectedPointV, 0, 3);
        glm::vec3 selectedPoint = surface->controlPoints[selectedPointU][selectedPointV];
        ImGui::Text("Position:");
        ImGui::SameLine();
        ImGui::DragFloat3("##Position", (float*)&selectedPoint,0.01f);
        surface->controlPoints[selectedPointU][selectedPointV] = selectedPoint;
        ImGui::End();
        
        // Recalculate
        surface->CalculateSurfacePoints();

        // Geometry of control points
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                controlPoints2[i * 4 + j] = surface->controlPoints[i][j];
            }
        }
        glBindVertexArray(cp_vao);
        glBindBuffer(GL_ARRAY_BUFFER, cp_vbo);
        glBufferData(GL_ARRAY_BUFFER, controlPoints2.size() * sizeof(glm::vec3), &controlPoints2[0], GL_STATIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Geometry of surfice points
        
        surfacePoints2.clear();
        normals.clear();
        for (int i = 0; i < surface->resolutionU; i++)
        {
            for (int j = 0; j < surface->resolutionV; j++)
            {
                glm::vec3 v1 = surface->surfacePoints[i][j];
                glm::vec3 v2 = surface->surfacePoints[i + 1][j];
                glm::vec3 v3 = surface->surfacePoints[i][j + 1];
                glm::vec3 v4 = surface->surfacePoints[i + 1][j + 1];

                surfacePoints2.push_back(v1);
                surfacePoints2.push_back(v2);
                surfacePoints2.push_back(v3);

                surfacePoints2.push_back(v3);
                surfacePoints2.push_back(v2);
                surfacePoints2.push_back(v4);

                glm::vec3 normal1 = glm::normalize(glm::cross(v2 - v1, v3 - v1));
                normals.push_back(normal1);
                normals.push_back(normal1);
                normals.push_back(normal1);

                glm::vec3 normal2 = glm::normalize(glm::cross(v3 - v4, v2 - v4));
                normals.push_back(normal2);
                normals.push_back(normal2);
                normals.push_back(normal2);
            }
        }

        glBindVertexArray(sp_vao);
        glBindBuffer(GL_ARRAY_BUFFER, sp_vbo);
        glBufferData(GL_ARRAY_BUFFER, surfacePoints2.size() * sizeof(glm::vec3), &surfacePoints2[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, sp_nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Calculate matrices
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            camera->OnRender(window);
        }
        glm::mat4 model = glm::mat4(1.0f); // identity matrix
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 MVP = projection * view * model;
        trinagleShader->SetMVP(MVP);

        // Clear frame
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
        phongShader->SetMVP(MVP);
        phongShader->SetLightPosition(glm::vec3(2.0, 2.0, 5.0)); //glm::vec3(2.0, 2.0, 5.0)
        phongShader->SetViewPos(camera->position);
        phongShader->SetObjectColor(glm::vec3(1.0f, 1.0f, 0.0f));
        phongShader->EnableShader();
        glBindVertexArray(sp_vao);
        glDrawArrays(GL_TRIANGLES, 0, surfacePoints2.size());
        phongShader->DisableShader();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Устанавливаем каркасный режим отрисовки

        // Очищаем массив точек
        surfacePoints2.clear();

        // Создаем сетку треугольников
        for (int i = 0; i < surface->resolutionU - 1; i++) {
            for (int j = 0; j < surface->resolutionV - 1; j++) {
                // Получаем четыре угла квадрата
                glm::vec3 bottomLeft = surface->surfacePoints[i][j];
                glm::vec3 bottomRight = surface->surfacePoints[i + 1][j];
                glm::vec3 topLeft = surface->surfacePoints[i][j + 1];
                glm::vec3 topRight = surface->surfacePoints[i + 1][j + 1];

                // Добавляем два треугольника для создания квадрата
                surfacePoints2.push_back(bottomLeft);
                surfacePoints2.push_back(bottomRight);
                surfacePoints2.push_back(topLeft);

                surfacePoints2.push_back(topLeft);
                surfacePoints2.push_back(bottomRight);
                surfacePoints2.push_back(topRight);
            }
        }

        // Обновляем VBO
        glBindBuffer(GL_ARRAY_BUFFER, sp_vbo);
        glBufferData(GL_ARRAY_BUFFER, surfacePoints2.size() * sizeof(glm::vec3), &surfacePoints2[0], GL_STATIC_DRAW);

        // Включаем шейдер для отрисовки
        trinagleShader->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
        trinagleShader->EnableShader();

        // Привязываем VAO и рисуем сетку
        glBindVertexArray(sp_vao);
        glDrawArrays(GL_TRIANGLES, 0, surfacePoints2.size());
        trinagleShader->DisableShader();

        // Возвращаем режим отрисовки полигонов обратно к заливке
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/
        // GUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       

        glfwSwapBuffers(window);
    }

   
    glfwTerminate();
    return 0;
}
