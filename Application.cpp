#include "Application.h"
#include "Triangle.h"
#include "Square.h"
#include <iostream>
#include <glm/vec3.hpp>

Application* Application::s_instance = nullptr;

Application::Application(int width, int height, const char* title)
    : window(nullptr),
    windowWidth(width),
    windowHeight(height),
    windowTitle(title),
    isRunning(false),
    lastFrameTime(0.0)
{
    s_instance = this;
}

Application::~Application()
{
    shutdown();
}

void Application::error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error [%d]: %s\n", error, description);
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
    if (s_instance)
    {
        s_instance->windowWidth = width;
        s_instance->windowHeight = height;
        glViewport(0, 0, width, height);

        if (s_instance->scene)
        {
            float aspect = width / (float)height;
            glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
            s_instance->scene->setProjectionMatrix(projection);
        }
    }
}

bool Application::initGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "ERROR: Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    std::cout << "GLFW initialized successfully\n";
    return true;
}

bool Application::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        fprintf(stderr, "ERROR: GLEW initialization failed: %s\n", glewGetErrorString(err));
        return false;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::cout << "GLEW initialized successfully\n";
    return true;
}

void Application::printSystemInfo()
{
    printf("=== System Information ===\n");
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLEW Version: %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("==========================\n");
}

bool Application::loadShaders(const char* vertexPath, const char* fragmentPath)
{
    shaderProgram = std::make_unique<ShaderProgram>();

    if (!shaderProgram->loadShader(vertexPath, fragmentPath))
    {
        fprintf(stderr, "ERROR: Failed to load shaders\n");
        return false;
    }

    std::cout << "Shaders loaded successfully\n";
    return true;
}

void Application::setupScene()
{
    scene = std::make_unique<Scene>();

    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 0, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    float aspect = windowWidth / (float)windowHeight;
    glm::mat4 projection = glm::perspective(45.0f, aspect, 0.1f, 100.0f);

    scene->setViewMatrix(view);
    scene->setProjectionMatrix(projection);

    Triangle* triangle1 = new Triangle();
    triangle1->setPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
    triangle1->setRotationSpeed(50.0f);
    triangle1->setColor(glm::vec3(0.5f, 0.0f, 0.5f));
    scene->addObject(triangle1);

    Square* square = new Square();
    square->setPosition(glm::vec3(0.5f, 0.0f, 0.0f));
    square->setColor(glm::vec3(0.0f, 0.5f, 1.0f));
    scene->addObject(square);

    Triangle* triangle2 = new Triangle();
    triangle2->setPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    triangle2->setRotationSpeed(30.0f);
    triangle2->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
    scene->addObject(triangle2);

    printf("Scene setup complete. Objects in scene: %zu\n", scene->getObjectCount());
}

bool Application::initialize()
{
    std::cout << "Initializing Application...\n";

    if (!initGLFW())
        return false;

    if (!initGLEW())
        return false;

    printSystemInfo();

    if (!loadShaders("vertex.glsl", "fragment.glsl"))
        return false;

    setupScene();

    isRunning = true;
    lastFrameTime = glfwGetTime();

    std::cout << "Application initialized successfully\n";
    return true;
}

void Application::run()
{
    if (!isRunning)
    {
        std::cerr << "ERROR: Application not initialized. Call initialize() first\n";
        return;
    }

    std::cout << "Application running. Press ESC to exit.\n";

    while (isOpen())
    {
        double currentTime = glfwGetTime();
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->update(deltaTime);

        scene->render(*shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::shutdown()
{
    std::cout << "Shutting down Application...\n";

    scene.reset();
    shaderProgram.reset();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
    isRunning = false;

    std::cout << "Application shutdown complete\n";
}