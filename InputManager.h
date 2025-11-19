#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Application;
class SceneManager;

class InputManager
{
private:
    GLFWwindow* window;
    Application* app;

    glm::vec2 lastMousePos;
    bool rightMousePressed;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

    void handleKeyPress(int key, int action, int mods);
    void handleMouseButton(int button, int action, int mods, double xpos, double ypos);
    void handleMouseMove(double xpos, double ypos);

    static InputManager* s_instance;

public:
    InputManager(GLFWwindow* window, Application* application);
    ~InputManager();

    void initialize();
    void processInput(float deltaTime);

    bool isRightMousePressed() const { return rightMousePressed; }
    glm::vec2 getLastMousePos() const { return lastMousePos; }
};