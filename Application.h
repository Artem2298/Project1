#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Scene.h"
#include "SceneManager.h"
#include "ShaderProgram.h"

class Application
{
private:
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    const char* windowTitle;

    //std::unique_ptr<Scene> scene;
    SceneManager sceneManager;
    std::unique_ptr<ShaderProgram> shaderProgram;

    bool isRunning;
    double lastFrameTime;

    bool initGLFW();
    bool initGLEW();
    bool loadShaders(const char* vertexPath, const char* fragmentPath);
    void setupScene(); 
    void printSystemInfo();

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);

    static Application* s_instance;

public:
    Application(int width, int height, const char* title);
    ~Application();

    bool initialize();
    void run();
    void shutdown();

    GLFWwindow* getWindow() const { return window; }
    //Scene* getScene() const { return scene.get(); }
    SceneManager& getSceneManager() { return sceneManager; }
    ShaderProgram* getShaderProgram() const { return shaderProgram.get(); }
    bool isOpen() const { return !glfwWindowShouldClose(window); }

    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
};