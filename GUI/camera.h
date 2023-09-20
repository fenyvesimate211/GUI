#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:

    Camera();

    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;

    int windowWidth;
    int windowHeight;
    float verticalFOV;
    float nearZ;
    float farZ;

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();

    float walkSpeed;
    float turnSpeed;
    void OnRender(GLFWwindow* window);
    bool isClick;
    int clickXpos;
    int clickYpos;
    
private:

    glm::vec3 GetTargetVector();
    glm::vec3 GetForwardVector();
    glm::vec3 GetRightVector();
    glm::vec3 GetUpVector();

};

#endif