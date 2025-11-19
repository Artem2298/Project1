#include <GL/glew.h>
#include <GLFW/glfw3.h>  

#include "WindowManager.h"
#include "Application.h"
#include <iostream>

WindowManager* WindowManager::s_instance = nullptr;

WindowManager::WindowManager(int width, int height, const char* title, Application* application)
    : window(nullptr), width(width), height(height), title(title), app(application)
{
    s_instance = this;
}

WindowManager::~WindowManager()
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    s_instance = nullptr;
}

bool WindowManager::initialize()
{
    if (!initGLFW())
    {
        return false;
    }

    if (!initGLEW())
    {
        return false;
    }

    printSystemInfo();

    return true;
}

bool WindowManager::initGLFW()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(errorCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    return true;
}

bool WindowManager::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << "\n";
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    return true;
}

void WindowManager::printSystemInfo()
{
    std::cout << "------------------------------------------------------------------------" << "\n";
    std::cout << "|OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "|GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "|Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "|Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "------------------------------------------------------------------------" << "\n";
}

void WindowManager::pollEvents()
{
    glfwPollEvents();
}

void WindowManager::swapBuffers()
{
    glfwSwapBuffers(window);
}

bool WindowManager::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void WindowManager::setShouldClose(bool value)
{
    glfwSetWindowShouldClose(window, value ? GLFW_TRUE : GLFW_FALSE);
}

void WindowManager::updateSize(int w, int h)
{
    width = w;
    height = h;
}

void WindowManager::errorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << "\n";
}

void WindowManager::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    WindowManager* wm = s_instance;
    if (!wm || !wm->app) return;

    glViewport(0, 0, width, height);
    wm->updateSize(width, height);

    Scene* currentScene = wm->app->getSceneManager().getCurrentScene();
    if (currentScene)
    {
        Camera* camera = currentScene->getCamera();
        if (camera)
        {
            float aspect = static_cast<float>(width) / static_cast<float>(height);
            camera->setPerspective(45.0f, aspect, 0.1f, 100.0f);
        }
    }
}