#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Scene.h"
#include "SceneManager.h"

class Application
{
private:
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    const char* windowTitle;

    SceneManager sceneManager;

    bool isRunning;
    double lastFrameTime;

    double lastMouseX;
    double lastMouseY;
    bool rightMousePressed;

    bool initGLFW();
    bool initGLEW();
    void setupScenes();

    void createScene1();
    void createScene2();
    void createScene3();
    void createScene4();

    void printSystemInfo();

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void processInput(float deltaTime);
    float randomFloat(float min, float max);

    static Application* s_instance;

public:
    Application(int width, int height, const char* title);
    ~Application();

    bool initialize();
    void run();
    void shutdown();

    GLFWwindow* getWindow() const { return window; }
    SceneManager& getSceneManager() { return sceneManager; }
    bool isOpen() const { return !glfwWindowShouldClose(window); }

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
};