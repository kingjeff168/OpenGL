/*
Last Date Modified: 1/9/2024

Description:

This file is to control the camera's orientation and distance from the origin (0, 0, 0).
There are six keys to control the camera's orientation and distance.
One key, g key, is to make the object move or not.

*/

// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix()
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix()
{
    return ProjectionMatrix;
}

// Camera parameters
float cameraRadius = 22.0f;
float cameraTheta = 0.5f; // initial directional angle
float cameraPhi = 0.8f;   // initial polar angle
glm::vec3 position; // camera position
glm::vec3 origin = glm::vec3(0, 0, 0); // viewpoint

// Initial Field of View
float initialFoV = 45.0f;

// Parameters to light control.
int moveControl = 0;
int previousGKeyStatus = GLFW_RELEASE;
int presentGKeyStatus;

void computeMatricesFromInputs()
{

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // GLFW_KEY_UP: closer to the origin.
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cameraRadius -= 0.1f;
        if (cameraRadius <= 0.0f)
        {
            cameraRadius = 0.0f;
        }
    }

    // GLFW_KEY_DOWN: farther from the origin.
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cameraRadius += 0.1f;
    }

    // GLFW_KEY_LEFT: left maintaining the radial distance from the origin.
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cameraTheta -= 0.008f;
        if (cameraTheta == 0)
        {
            cameraTheta = 6.28f;
        }
    }

    // GLFW_KEY_RIGHT: right maintaining the radial distance from the origin.
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cameraTheta += 0.008f;
        if (cameraTheta >= 6.28f)
        {
            cameraTheta = 0.0f;
        }
    }

    // GLFW_KEY_D: rotates the camera down.
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPhi += 0.005f;
        if (cameraPhi >= 3.13f)
        {
            cameraPhi = 3.13f;
        }   
    }

    // GLFW_KEY_U: rotates the camera up.
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        cameraPhi -= 0.005f;
        if (cameraPhi < 0.01f)
        {
            cameraPhi = 0.01f;
        }
    }

    // GLFW_KEY_G: toggles the movements of objects.
    // It remembers the previous key's status and get the present key's status. Only toggles the movement when 
    // (presentGKeyStatus == GLFW_PRESS) && (previousGKeyStatus == GLFW_RELEASE).
    presentGKeyStatus = glfwGetKey(window, GLFW_KEY_G);
    if ((presentGKeyStatus == GLFW_PRESS) && (previousGKeyStatus == GLFW_RELEASE))
    {
        moveControl = !moveControl;
    }
    previousGKeyStatus = presentGKeyStatus;
    

    // Calculate the coordinates of the camera.
    float x = cameraRadius * sin(cameraPhi) * cos(cameraTheta);
    float y = cameraRadius * sin(cameraPhi) * sin(cameraTheta);
    float z = cameraRadius * cos(cameraPhi);
    position = glm::vec3(x, y, z);

    // Up vector
    glm::vec3 up = glm::vec3(0, 0, 1);

    float FoV = initialFoV;

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    ViewMatrix = glm::lookAt(position, origin, up);

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}