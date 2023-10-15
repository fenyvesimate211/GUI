#include "surface.h"


Surface::Surface()
{
    // Control Points
    controlPoints = std::vector<std::vector<glm::vec3>>(4, std::vector<glm::vec3>(4));

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
    resolutionU = 50; // Number of steps in the u direction
    resolutionV = 50; // Number of steps in the v direction

    // Surface Points
    surfacePoints = std::vector<std::vector<glm::vec3>>(resolutionU + 1, std::vector<glm::vec3>(resolutionV + 1));
}