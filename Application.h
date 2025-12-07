#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Scene.h"
#include "SceneManager.h"
#include "LightObject.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "SceneFactory.h"

class Application
{
private:
    SceneManager sceneManager;

    bool isRunning;
    double lastFrameTime;

    void setupScenes();

    static Application* s_instance;
    std::unique_ptr<WindowManager> windowManager;
    std::unique_ptr<InputManager> inputManager;
    SceneFactory sceneFactory;

public:
    Application(int width, int height, const char* title);
    ~Application();

    bool initialize();
    void run();
    void shutdown();

    GLFWwindow* getWindow() const { return windowManager->getWindow(); }
    SceneManager& getSceneManager() { return sceneManager; }
    bool isOpen() const { return !windowManager->shouldClose(); }

    int getWindowWidth() const { return windowManager->getWidth(); }
    int getWindowHeight() const { return windowManager->getHeight(); }
};