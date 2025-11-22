#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include "Application.h"
#include "TranslateTransform.h"
#include "RotateTransform.h"
#include "ScaleTransform.h"
#include "DynamicRotateTransform.h"
#include "Camera.h"
#include "Light.h"
#include "DrawableObject.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/gtc/constants.hpp>
#include "SpotLight.h"
#include "SpotLightTracker.h"
#include "Texture.h"

Application* Application::s_instance = nullptr;

Application::Application(int width, int height, const char* title)
    : windowManager(std::make_unique<WindowManager>(width, height, title, this)),
    inputManager(nullptr),
    isRunning(true),
    lastFrameTime(0.0)
{
    s_instance = this;
}

Application::~Application()
{
    shutdown();
}

void Application::setupScenes()
{
    std::cout << "Setting up scenes..." << std::endl;

    float aspectRatio = (float)windowManager->getWidth() / (float)windowManager->getHeight();

    Scene* scene1 = sceneFactory.createScene(1, aspectRatio);
    sceneManager.addScene(1, scene1);

    Scene* scene2 = sceneFactory.createScene(2, aspectRatio);
    sceneManager.addScene(2, scene2);

    Scene* scene3 = sceneFactory.createScene(3, aspectRatio);
    sceneManager.addScene(3, scene3);

    Scene* scene4 = sceneFactory.createScene(4, aspectRatio);
    sceneManager.addScene(4, scene4);

    sceneManager.switchScene(4);
}

bool Application::initialize()
{
    if (!windowManager->initialize())
    {
        return false;
    }

    inputManager = std::make_unique<InputManager>(windowManager->getWindow(), this);
    inputManager->initialize();

    setupScenes();

    return true;
}

void Application::run()
{
    std::cout << "Application::run() started" << std::endl;

    while (isRunning && !windowManager->shouldClose())
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        inputManager->processInput(deltaTime);

        Scene* currentScene = sceneManager.getCurrentScene();
        if (currentScene) {
            currentScene->update(deltaTime);
        }

        inputManager->processInput(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (currentScene) {
            currentScene->render();
        }

        windowManager->swapBuffers();
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    windowManager.reset();
}