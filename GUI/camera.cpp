#include <stdio.h>

#include <GLFW/glfw3.h>

#include "camera.h"

using namespace glm;



Camera::Camera()
{
    position = vec3(0.0f, 0.0f, 0.0f);
    horizontalAngle = 0.0f;
    verticalAngle = 0.0f;

    windowWidth  = 16;
    windowHeight = 9;
    verticalFOV = 45.0f;
    nearZ = 0.1f;
    farZ = 100.0f;
    
    walkSpeed = 5.0f;
    turnSpeed = 0.001f;
    isClick = false;
    clickXpos = 0;
    clickYpos = 0;
}



void Camera::OnRender(GLFWwindow* window)
{
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    static double lastXpos = xpos;
    static double lastYpos = ypos;
    double deltaX = xpos - lastXpos;
    double deltaY = ypos - lastYpos;
    lastXpos = xpos;
    lastYpos = ypos;

    isClick = false;
    static bool isClicking = false;
    static bool isMoved = false;

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS)
    {
        if (!isClicking)
        {
            clickXpos = xpos;
            clickYpos = ypos;
            isClicking = true;
        }
        if (abs(xpos-clickXpos) > 2 || abs(ypos-clickYpos) > 2)
        {
            isMoved = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        horizontalAngle -= turnSpeed * (float)deltaX;
        verticalAngle -= turnSpeed * (float)deltaY;
        verticalAngle = clamp(verticalAngle, -half_pi<float>(), half_pi<float>());
    }
    if (state == GLFW_RELEASE)
    {
        if(isClicking)
        {
            if(isMoved)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                isClick = true;
            isClicking = false;
        }
        isMoved = false;
    }


    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    float moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
            moveSpeed = walkSpeed * 2;
        else
            moveSpeed = walkSpeed;

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)){
            position += GetForwardVector() * deltaTime * moveSpeed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)){
        position -= GetForwardVector() * deltaTime * moveSpeed;
    }
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)){
        position += GetRightVector() * deltaTime * moveSpeed;
    }
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)){
        position -= GetRightVector() * deltaTime * moveSpeed;
    }
    // Move up
    if (glfwGetKey(window, GLFW_KEY_SPACE)){
        position += vec3(0, 1, 0) * deltaTime * moveSpeed;
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)){
        position -= vec3(0, 1, 0) * deltaTime * moveSpeed;
    }

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}





vec3 Camera::GetTargetVector()
{
    return vec3(
        cos(verticalAngle) * sin(horizontalAngle), 
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
}

vec3 Camera::GetForwardVector()
{
    return vec3(
        sin(horizontalAngle), 
        0,
        cos(horizontalAngle)
    );
}

vec3 Camera::GetRightVector()
{
    return vec3(
        sin(horizontalAngle - half_pi<float>()), 
        0,
        cos(horizontalAngle - half_pi<float>())
    );
}

vec3 Camera::GetUpVector()
{
    return cross(GetRightVector(), GetTargetVector());
}

mat4 Camera::GetViewMatrix()
{
    return lookAt(position, position+GetTargetVector(), GetUpVector());
}

mat4 Camera::GetProjectionMatrix()
{
    return perspective(
        radians(verticalFOV),
        (float)windowWidth / (float)windowHeight,
        nearZ, farZ);
}