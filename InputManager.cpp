#include <GL/glew.h>
#include <GLFW/glfw3.h>  

#include "InputManager.h"
#include "Application.h"
#include "Scene.h"
#include "Camera.h"
#include "DrawableObject.h"
#include <iostream>

InputManager* InputManager::s_instance = nullptr;

InputManager::InputManager(GLFWwindow* window, Application* application)
    : window(window), app(application), lastMousePos(0.0, 0.0), rightMousePressed(false)
{
    s_instance = this;
}

InputManager::~InputManager()
{
    s_instance = nullptr;
}

void InputManager::initialize()
{
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
}

void InputManager::processInput(float deltaTime)
{
    Scene* currentScene = app->getSceneManager().getCurrentScene();
    if (!currentScene) return;

    Camera* camera = currentScene->getCamera();
    if (!camera) return;

    const float moveSpeed = 5.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 position = camera->getEye();
        glm::vec3 direction = camera->getTarget();
        camera->setPosition(position + direction * moveSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 position = camera->getEye();
        glm::vec3 direction = camera->getTarget();
        camera->setPosition(position - direction * moveSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm::vec3 position = camera->getEye();
        glm::vec3 direction = camera->getTarget();
        glm::vec3 up = camera->getUp();
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        camera->setPosition(position - right * moveSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm::vec3 position = camera->getEye();
        glm::vec3 direction = camera->getTarget();
        glm::vec3 up = camera->getUp();
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        camera->setPosition(position + right * moveSpeed);
    }
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputManager* input = s_instance;
    if (!input || !input->app) return;

    if (action == GLFW_PRESS)
    {
        input->handleKeyPress(key, action, mods);
    }
}

void InputManager::handleKeyPress(int key, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_1)
    {
        app->getSceneManager().switchScene(1);
        std::cout << "Switched to Scene 1" << std::endl;
    }
    else if (key == GLFW_KEY_2)
    {
        app->getSceneManager().switchScene(2);
        std::cout << "Switched to Scene 2" << std::endl;
    }
    else if (key == GLFW_KEY_3)
    {
        app->getSceneManager().switchScene(3);
        std::cout << "Switched to Scene 3" << std::endl;
    }
    else if (key == GLFW_KEY_4)
    {
        app->getSceneManager().switchScene(4);
        std::cout << "Switched to Scene 4" << std::endl;
    }
    else if (key == GLFW_KEY_F)
    {
        Scene* currentScene = app->getSceneManager().getCurrentScene();
        if (currentScene)
        {
            SpotLight* flashlight = currentScene->getSpotLight();
            if (flashlight)
            {
                bool currentState = flashlight->isEnabled();
                flashlight->setEnabled(!currentState);
                std::cout << "\nFlashlight: " << (flashlight->isEnabled() ? "ON" : "OFF") << std::endl;
            }
            else
            {
                std::cout << "No flashlight in current scene!" << std::endl;
            }
        }
    }
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    InputManager* input = s_instance;
    if (!input || !input->app) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    input->handleMouseButton(button, action, mods, xpos, ypos);
}

void InputManager::handleMouseButton(int button, int action, int mods, double xpos, double ypos)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        GLint x = static_cast<GLint>(xpos);
        GLint y = static_cast<GLint>(ypos);

        int windowHeight = 0;
        glfwGetWindowSize(window, nullptr, &windowHeight);
        int newY = windowHeight - y;

        GLfloat depth;
        GLuint index;
        glReadPixels(x, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels(x, newY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        printf("\n-----------------------------\n");
        printf("|Screen position: (%d, %d)\n", x, y);
        printf("|OpenGL position: (%d, %d)\n", x, newY);
        printf("|Depth: %f\n", depth);
        printf("|Object ID: %u\n", index);
        printf("-----------------------------\n\n");

        if (index == 1)
        {
            Scene* scene = app->getSceneManager().getCurrentScene();
            if (!scene) return;

            Camera* camera = scene->getCamera();
            if (!camera) return;

            int windowWidth = 0;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);

            glm::mat4 viewMatrix = camera->getCamera();
            glm::mat4 projectionMatrix = camera->getProjectionMatrix();
            glm::vec4 viewport = glm::vec4(0, 0, windowWidth, windowHeight);

            glm::vec3 worldPos = glm::unProject(
                glm::vec3(x, newY, depth),
                viewMatrix,
                projectionMatrix,
                viewport
            );

            scene->putTree(worldPos);

            printf("\n-------- teren PLANTED --------\n");
            printf("World position: (%.2f, %.2f, %.2f)\n",
                worldPos.x, worldPos.y, worldPos.z);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rightMousePressed = true;
        lastMousePos = glm::vec2(xpos, ypos);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rightMousePressed = false;
    }
}

void InputManager::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager* input = s_instance;
    if (!input || !input->app) return;

    input->handleMouseMove(xpos, ypos);
}

void InputManager::handleMouseMove(double xpos, double ypos)
{
    if (!rightMousePressed)
    {
        lastMousePos = glm::vec2(xpos, ypos);
        return;
    }

    double xOffset = xpos - lastMousePos.x;
    double yOffset = lastMousePos.y - ypos;
    lastMousePos = glm::vec2(xpos, ypos);

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    Scene* currentScene = app->getSceneManager().getCurrentScene();
    if (!currentScene) return;

    Camera* camera = currentScene->getCamera();
    if (!camera) return;

    camera->updateDirection(static_cast<float>(xOffset), static_cast<float>(yOffset));
}